#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "keybinds_temp.h"
#include "intmath.h"
#include "flags.h"

// OPERATIONS
#define OP_NONE 0
#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_DIV 4
#define OP_AND 5
#define OP_OR 6
#define OP_XOR 7
#define OP_LSH 8
#define OP_RSH 9

// COLORS
#define C_MAGENTA "\033[35m"
#define C_LMAGENTA "\033[95m"
#define C_LBLACK "\033[90m"
#define C_RESET "\033[0m"

#define ALL_ONES 0xFFFFFFFFFFFFFFFF

char getch() {
	struct termios oldt, newt;
	char ch;

	tcgetattr(STDIN_FILENO, &oldt);         
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);       
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
	return ch;
}

typedef struct {
	uint8_t base;
	uint8_t bits;
	uint8_t isSigned;
	uint8_t op;
	uint8_t grouping;
	uint8_t showLogMode;
	uint64_t mainReg;
	uint64_t inputReg;
	uint64_t extReg;
	Flags* flags;
} Context;

// convert a numerical value to a digit
// letters for hex are in uppercase, since it's for display
char value2Digit(uint8_t value) {
	if (value < 10) return '0' + value;
	else return 'A' - 10 + value;
}

// convert a digit to a numerical value
// letters for hex are accepted in lowercase, since it's for input
// for non-digit char's returns -1
uint8_t digit2Value(char digit) {
	if ('0' <= digit && digit <= '9') return digit - '0';
	else if ('a' <= digit && digit <= 'f') return digit - 'a' + 10;
	else return -1;
}

// gets thuint8_te digit of the number in a base at the position pos
// 0'th position - least significant, increases with digit significance
uint8_t getValueAtPosition(uint64_t number, uint8_t pos, uint8_t base) {
	for (int i = 0; i < pos; i++) number /= base;
	return number % base;
}

// sets all bits overflowing the current bitness to 0
// e. g. (word) 0x123456789ABCDEF0 -> 0x000000000000DEF0
void truncOverflowing(Context* ctx, uint64_t* number) {
	if (ctx->bits == 64) return;
	*number &= ~(ALL_ONES << ctx->bits);
}

// negation in two's complement
void negate(Context* ctx, uint64_t* number) {
	*number *= -1;
	truncOverflowing(ctx, number);
}

// true if the mode is signed and the number is negative, else false
uint8_t isSignedAndNegative(Context* ctx, uint64_t number) {
	return ctx->isSigned && (number & (1ull << (ctx->bits - 1)));
}

void clearLine() {
	printf("\r\033[K");
}

void printFlag(int8_t value, char flagLetter) {
	if (value == 1) printf(C_LMAGENTA);
	else printf(C_LBLACK);

	if (value != -1) printf("%c", flagLetter);
	else printf("-");
}

void printFlags(Flags* flags) {
	printFlag(flags->sf, 'S');
	printFlag(flags->zf, 'Z');
	printFlag(flags->cf, 'C');
	printFlag(flags->of, 'O');
}

void clearFlags(Flags* flags) {
	flags->sf = -1;
	flags->zf = -1;
	flags->cf = -1;
	flags->of = -1;
}

void printNumber(Context* ctx, uint64_t number) {
	if (ctx->base == 10 && isSignedAndNegative(ctx, number)) {
		negate(ctx, &number);
		printf("-");
	}

	printf(C_LBLACK);

	uint8_t n_digits = ceil(ctx->bits * log(2) / log(ctx->base));

	uint8_t groupSize = 1;
	if (ctx->base == 2) {
		groupSize = 4;
	} else if (ctx->base == 10) {
		groupSize = 3;
	} else if (ctx->base == 16) {
		groupSize = 2;
	} else ctx->grouping = 0;

	for (int pos = n_digits - 1; pos >= 0; pos--) {
		char value = getValueAtPosition(number, pos, ctx->base);
		if (value) printf(C_LMAGENTA);

		char digit = value2Digit(value);
		printf("%c", digit);

		if (ctx->grouping && !(pos % groupSize) && pos) printf(" ");
	}

	printf(C_RESET);
}

void printOp(uint8_t op) {
	if (op == OP_ADD) printf("+");
	else if (op == OP_SUB) printf("-");
	else if (op == OP_MUL) printf("*");
	else if (op == OP_DIV) printf("/");
	else if (op == OP_AND) printf("&");
	else if (op == OP_OR) printf("|");
	else if (op == OP_XOR) printf("^");
	else if (op == OP_LSH) printf("<<");
	else if (op == OP_RSH) printf(">>");
}

void printBase(uint8_t base) {
	if (base == 2) printf("BIN");
	else if (base == 3) printf("TRN");
	else if (base == 4) printf("QTR");
	else if (base == 5) printf("QIN");
	else if (base == 6) printf("SEN");
	else if (base == 7) printf("SPT");
	else if (base == 8) printf("OCT");
	else if (base == 9) printf("NON");
	else if (base == 10) printf("DEC");
	else if (base == 11) printf("UND");
	else if (base == 12) printf("DUD");
	else if (base == 13) printf("TRD");
	else if (base == 14) printf("TDD");
	else if (base == 15) printf("PND");
	else if (base == 16) printf("HEX");
}

// prints the info and all numbers
void print(Context* ctx) {
	clearLine();
	printf("[" C_MAGENTA);
	if (ctx->bits == 8) printf("BYTE");
	else if (ctx->bits == 16) printf("WORD");
	else if (ctx->bits == 32) printf("DWORD");
	else if (ctx->bits == 64) printf("QWORD");
	printf(" ");
	printBase(ctx->base);
	printf(" ");
	if (ctx->isSigned) printf("S");
	else printf("U");
	printf(" ");
	printFlags(ctx->flags);
	printf(C_RESET "] ");
	
	if (ctx->showLogMode == 1) {
		float log = log2(ctx->mainReg);
		log *= 10;
		log = (float) floor(log);
		log /= 10;
		printf(C_MAGENTA"(log2 = %.1f) "C_RESET, log);
	} else if (ctx->showLogMode == 2) {
		uint8_t n_bits;

		if (ctx->mainReg == 0) n_bits = 0;
		else {
			n_bits = floor(log2(ctx->mainReg)) + 1;
		}

		printf(C_MAGENTA"(%d bits) "C_RESET, n_bits);
	}

	printf(" ");

	if (ctx->extReg != 0) {
		printNumber(ctx, ctx->extReg);
		printf(":");
	}

	printNumber(ctx, ctx->mainReg);

	if (ctx->op == OP_NONE) return;

	printf(" ");
	printOp(ctx->op);
	printf(" ");

	printNumber(ctx, ctx->inputReg);
}

void addDigit(Context* ctx, uint64_t* number, uint64_t digitValue) {
	uint8_t bits = ctx->bits;
	// the sign bit doesn't count
	if (ctx-> base == 10 && ctx->isSigned) bits--;

	uint8_t wasNegated = 0;
	if (ctx-> base == 10 && isSignedAndNegative(ctx, *number)) {
		negate(ctx, number);
		wasNegated = 1;
	}

	uint64_t maxVal;
	if (bits == 64) maxVal = ALL_ONES;
	else maxVal = (1ull << bits) - 1;

	if (*number > (maxVal - digitValue) / ctx->base) { // overflow will occur
		if (wasNegated) negate(ctx, number);
		return;
	}

	*number = *number * ctx->base + digitValue;

	if (wasNegated) negate(ctx, number);

}

void eraseDigit(Context* ctx, uint64_t* number) {
	if (ctx->base == 10 && isSignedAndNegative(ctx, *number)) {
		negate(ctx, number);
		*number /= ctx->base;
		negate(ctx, number);
	} else {
		*number /= ctx->base;
	}
}

void setBits(Context* ctx, uint8_t bits, uint8_t preserveSign) {
	if (!preserveSign) {
		if (bits > ctx->bits) {
			ctx->mainReg |= ctx->extReg << ctx->bits;
			ctx->extReg = 0;
		}
		ctx->bits = bits;
		truncOverflowing(ctx, &ctx->mainReg);
	} else {
		uint8_t isNegative = isSignedAndNegative(ctx, ctx->mainReg);
		if (isNegative) negate(ctx, &ctx->mainReg);
		ctx->bits = bits;
		if (isNegative) negate(ctx, &ctx->mainReg);
	}
}

void performOperation(Context* ctx) {
	if (ctx->op == OP_ADD)
		intmath_add(&ctx->mainReg, ctx->inputReg, ctx->bits, ctx->flags);
	else if (ctx->op == OP_SUB)
		intmath_sub(&ctx->mainReg, ctx->inputReg, ctx->bits, ctx->flags);
	else if (ctx->op == OP_AND)
		intmath_and(&ctx->mainReg, ctx->inputReg, ctx->bits, ctx->flags);
	else if (ctx->op == OP_OR)
		intmath_or(&ctx->mainReg, ctx->inputReg, ctx->bits, ctx->flags);
	else if (ctx->op == OP_XOR)
		intmath_sub(&ctx->mainReg, ctx->inputReg, ctx->bits, ctx->flags);
	else if (ctx->op == OP_MUL)
		intmath_mul(&ctx->mainReg,
			&ctx->extReg,
			ctx->inputReg,
			ctx->isSigned,
			ctx->bits,
			ctx->flags);
	else if (ctx->op == OP_DIV) {
		if (ctx->inputReg == 0) return;
		intmath_div(&ctx->mainReg,
			&ctx->extReg,
			ctx->inputReg,
			ctx->isSigned,
			ctx->bits,
			ctx->flags);
	} else if (ctx->op == OP_LSH)
		intmath_lsh(&ctx->mainReg, ctx->inputReg, ctx->bits, ctx->flags);
	else if(ctx->op == OP_RSH)
		intmath_rsh(&ctx->mainReg, ctx->inputReg, ctx->isSigned, ctx->bits, ctx->flags);

	ctx->inputReg = 0;
	ctx->op = OP_NONE;
	truncOverflowing(ctx, &ctx->mainReg);
}

void basePlus(Context* ctx) {
	if (ctx->base < 16) ctx->base++; 
}

void baseMinus(Context* ctx) {
	if (ctx->base > 2) ctx->base--; 
}

void printHelp() {
	printf("More information on https://github.com/phitazero/phicalc\n\nKeybinds:\n");
	printKeybinds(); // from keybinds_temp.h
}

int main(int argc, char* argv[]) {
	// if -h or --help is passed
	if (argc >= 2 && !(strcmp(argv[1], "-h") && strcmp(argv[1], "--help"))) {
		printHelp();
		return 0;
	}

	// initialize the flags to being undefined
	Flags flags;
	clearFlags(&flags);

	// initialize the context
	Context ctx;
	ctx.mainReg = 0;
	ctx.inputReg = 0;
	ctx.extReg = 0;
	ctx.base = 10;
	ctx.bits = 32;
	ctx.grouping = 0;
	ctx.op = OP_NONE;
	ctx.isSigned = 1;
	ctx.showLogMode = 0;
	ctx.flags = &flags;

	// initial print
	print(&ctx);

	// handling input
	char input;
	do {
		input = getch();
		uint8_t wasUpdated = 1;

		// BASES
		if (input == KEY_TO_BIN) ctx.base = 2;
		else if (input == KEY_TO_DEC) ctx.base = 10;
		else if (input == KEY_TO_HEX) ctx.base = 16;
		else if (input == KEY_BASE_PLUS) basePlus(&ctx);
		else if (input == KEY_BASE_MINUS) baseMinus(&ctx);

		// SIGN
		else if (input == KEY_SIGN && ctx.isSigned)
			if (ctx.op == OP_NONE) negate(&ctx, &ctx.mainReg);
			else negate(&ctx, &ctx.inputReg);

		// SIGNEDNESS
		else if (input == KEY_SIGNEDNESS) ctx.isSigned ^= 1;

		// LOGARITHM
		else if (input == KEY_LOGARITHM) {
			ctx.showLogMode++;
			ctx.showLogMode %= 3;
		}

		// BACKSPACE
		else if (input == KEY_ERASE) {
			if (ctx.op == OP_NONE) eraseDigit(&ctx, &ctx.mainReg);
			else if (ctx.inputReg != 0) eraseDigit(&ctx, &ctx.inputReg);
			else ctx.op = OP_NONE;
		}

		// CLEAR
		else if (input == KEY_CLEAR) {
			ctx.mainReg = 0;
			ctx.inputReg = 0;
			ctx.extReg = 0;
			ctx.op = OP_NONE;
			clearFlags(ctx.flags);
		}

		// DIGIT GROUPING
		else if (input == KEY_GROUPING) ctx.grouping ^= 1;

		// CLEAR EXTENSION REGISTER
		else if (input == KEY_EXT_CLEAR) ctx.extReg = 0;

		// COPY EXTENSION REGISTER TO NUMBER REGISTER
		else if (input == KEY_COPY_EXT2NUM) ctx.mainReg = ctx.extReg;

		// BITS
		else if (input == KEY_TO_BYTE && ctx.op == OP_NONE) setBits(&ctx, 8, 0);
		else if (input == KEY_TO_WORD && ctx.op == OP_NONE) setBits(&ctx, 16, 0);
		else if (input == KEY_TO_DWORD && ctx.op == OP_NONE) setBits(&ctx, 32, 0);
		else if (input == KEY_TO_QWORD && ctx.op == OP_NONE) setBits(&ctx, 64, 0);
		else if (input == KEY_TO_BYTE_PS && ctx.op == OP_NONE) setBits(&ctx, 8, 1);
		else if (input == KEY_TO_WORD_PS && ctx.op == OP_NONE) setBits(&ctx, 16, 1);
		else if (input == KEY_TO_DWORD_PS && ctx.op == OP_NONE) setBits(&ctx, 32, 1);
		else if (input == KEY_TO_QWORD_PS && ctx.op == OP_NONE) setBits(&ctx, 64, 1);

		// OPERATIONS
		else if (input == '+') ctx.op = OP_ADD;
		else if (input == '-') ctx.op = OP_SUB;
		else if (input == '*') ctx.op = OP_MUL;
		else if (input == '/') ctx.op = OP_DIV;
		else if (input == '&') ctx.op = OP_AND;
		else if (input == '|') ctx.op = OP_OR;
		else if (input == '^') ctx.op = OP_XOR;
		else if (input == '<') ctx.op = OP_LSH;
		else if (input == '>') ctx.op = OP_RSH;
		
		// PERFORM AN OPERATION
		else if (ctx.op != OP_NONE && input == KEY_ENTER)
			performOperation(&ctx);

		// DIGIT INPUT
		else if (('0' <= input && input <= '9') || ('a' <= input && input <= 'f')) {
			uint8_t value = digit2Value(input);
			if (value >= ctx.base) continue;
			if (ctx.op == OP_NONE) addDigit(&ctx, &ctx.mainReg, value);
			else addDigit(&ctx, &ctx.inputReg, value);
		}

		else wasUpdated = 0;
		if (wasUpdated) print(&ctx);

	} while (input != KEY_EXIT);

	printf("\n");

	return 0;
}
