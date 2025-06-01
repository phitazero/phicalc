#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// TYPES
#define ull unsigned long long

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

// KEYBINDS
#define KEY_TO_BYTE 'q'
#define KEY_TO_WORD 'w'
#define KEY_TO_DWORD 'r'
#define KEY_TO_QWORD 't'
#define KEY_TO_BYTE_PS 'Q' // PS means Preserve Sign
#define KEY_TO_WORD_PS 'W'
#define KEY_TO_DWORD_PS 'R'
#define KEY_TO_QWORD_PS 'T'
#define KEY_TO_BIN 'i'
#define KEY_TO_DEC 'o'
#define KEY_TO_HEX 'p'
#define KEY_GROUPING ' '
#define KEY_SIGN 's'
#define KEY_SIGNEDNESS 'S'
#define KEY_ENTER '\n'
#define KEY_CLEAR 'C'
#define KEY_ERASE 127 // backspace
#define KEY_COPY_BUFFER 'D'
#define KEY_EXIT 'X'

// COLORS
#define C_MAGENTA "\033[35m"
#define C_LMAGENTA "\033[95m"
#define C_LBLACK "\033[90m"
#define C_YELLOW "\033[33m"
#define C_RESET "\033[0m"

#define ALL_ONES 0xFFFFFFFFFFFFFFFF

int getch() {
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);         
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);       
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
	return ch;
}

typedef struct {
	int base;
	int bits;
	int isSigned;
	int op;
	int grouping;
	ull number;
	ull buffer;
} Context;

// convert a numerical value to a digit
// letters for hex are in uppercase, since it's for display
char value2Digit(int value) {
	if (value < 10) return '0' + value;
	else return 'A' - 10 + value;
}

// convert a digit to a numerical value
// letters for hex are accepted in lowercase, since it's for input
// for non-digit chars returns -1
int digit2Value(char digit) {
	if ('0' <= digit && digit <= '9') return digit - '0';
	else if ('a' <= digit && digit <= 'f') return digit - 'a' + 10;
	else return -1;
}

int getValueAtPosition(ull number, int pos, int base) {
	for (int i = 0; i < pos; i++) number /= base;
	return number % base;
}

void truncOverflowing(Context* ctx, ull* number) {
	if (ctx->bits == 64) return;
	*number &= ~(ALL_ONES << ctx->bits);
}

void negate(Context* ctx, ull* number) {
	*number *= -1;
	truncOverflowing(ctx, number);
}

int isSignedAndNegative(Context* ctx, ull number) {
	return ctx->isSigned && (number & (1ull << (ctx->bits - 1)));
}

void printNumberDec(Context* ctx, ull number) {
	printf(C_LMAGENTA);
	if (number == 0) {
		printf("0" C_RESET);
		return; // further logic relies on the number being non-zero
	}

	if (isSignedAndNegative(ctx, number)) {
		negate(ctx, &number);
		printf("-");
	}

	int n_digits = floor(log10(number));
	for (int pos = n_digits; pos >= 0; pos--) {
		char value = getValueAtPosition(number, pos, 10);

		char digit = value2Digit(value);
		printf("%c", digit);
		
		if (ctx->grouping && !(pos % 3) && pos) printf(" ");
	}
	printf(C_RESET);
}

void printNumber(Context* ctx, ull number) {
	if (ctx->base == 10) {
		printNumberDec(ctx, number);
		return;
	}

	printf(C_LBLACK);

	int n_digits;
	int groupSize;
	if (ctx->base == 2) {
		n_digits = ctx->bits;
		groupSize = 4;
	} else if (ctx->base == 16) {
		n_digits = ctx->bits / 4;
		groupSize = 2;
	}

	for (int pos = n_digits - 1; pos >= 0; pos--) {
		char value = getValueAtPosition(number, pos, ctx->base);
		if (value) printf(C_LMAGENTA);

		char digit = value2Digit(value);
		printf("%c", digit);

		if (ctx->grouping && !(pos % groupSize) && pos) printf(" ");
	}

	printf(C_RESET);
}

void printOp(Context* ctx) {
	if (ctx->op == OP_ADD) printf("+");
	else if (ctx->op == OP_SUB) printf("-");
	else if (ctx->op == OP_MUL) printf("*");
	else if (ctx->op == OP_DIV) printf("/");
	else if (ctx->op == OP_AND) printf("&");
	else if (ctx->op == OP_OR) printf("|");
	else if (ctx->op == OP_XOR) printf("^");
	else if (ctx->op == OP_LSH) printf("<<");
	else if (ctx->op == OP_RSH) printf(">>");
}

void print(Context* ctx) {
	printf("\r\033[K"); // clear the line and return to its beginning
	printf("[" C_MAGENTA);
	if (ctx->bits == 8) printf("BYTE");
	else if (ctx->bits == 16) printf("WORD");
	else if (ctx->bits == 32) printf("DWORD");
	else if (ctx->bits == 64) printf("QWORD");
	printf(" ");
	if (ctx->base == 2) printf("BIN");
	else if (ctx->base == 10) printf("DEC");
	else if (ctx->base == 16) printf("HEX");
	printf(" ");
	if (ctx->isSigned) printf("S");
	else printf("U");
	printf(C_RESET "]  ");
	
	printNumber(ctx, ctx->number);

	if (ctx->op == OP_NONE) return;

	printf(" ");
	printOp(ctx);
	printf(" ");

	printNumber(ctx, ctx->buffer);
}

void addDigit(Context* ctx, ull* number, ull digitValue) {
	int bits = ctx->bits;
	// the sign bit doesn't count
	if (ctx-> base == 10 && ctx->isSigned) bits--;

	ull maxVal;
	if (bits == 64) maxVal = ALL_ONES;
	else maxVal = (1ull << bits) - 1;

	if (*number > (maxVal - digitValue) / ctx->base) return; // overflow will occur

	*number = *number * ctx->base + digitValue;

}

void eraseDigit(Context* ctx, ull* number) {
	if (isSignedAndNegative(ctx, *number)) {
		negate(ctx, number);
		*number /= ctx->base;
		negate(ctx, number);
	} else {
		*number /= ctx->base;
	}
}

void setBits(Context* ctx, int bits, int preserveSign) {
	if (!preserveSign) {
		ctx->bits = bits;
		truncOverflowing(ctx, &ctx->number);
	} else {
		int isNegative = isSignedAndNegative(ctx, ctx->number);
		if (isNegative) negate(ctx, &ctx->number);
		ctx->bits = bits;
		if (isNegative) negate(ctx, &ctx->number);
	}
}

void performOperation(Context* ctx) {
	if (ctx->op == OP_ADD) ctx->number += ctx->buffer;
	else if (ctx->op == OP_SUB) ctx->number -= ctx->buffer;
	else if (ctx->op == OP_MUL) ctx->number *= ctx->buffer;
	else if (ctx->op == OP_AND) ctx->number &= ctx->buffer;
	else if (ctx->op == OP_OR) ctx->number |= ctx->buffer;
	else if (ctx->op == OP_XOR) ctx->number ^= ctx->buffer;
	else if (ctx->op == OP_DIV) {
		if (ctx->buffer == 0) return;

		int isResNegative = 0;
		if (isSignedAndNegative(ctx, ctx->number)) {
			negate(ctx, &ctx->number);
			isResNegative ^= 1;
		}
		if (isSignedAndNegative(ctx, ctx->buffer)) {
			negate(ctx, &ctx->buffer);
			isResNegative ^= 1;
		}

		ctx->number /= ctx->buffer;
		if (isResNegative) negate(ctx, &ctx->number);
	} else if (ctx->op == OP_LSH) {
		if (isSignedAndNegative(ctx, ctx->buffer)) return;

		if (ctx->buffer >= ctx->bits) ctx->number = 0;
		else ctx->number <<= ctx->buffer;

	} else if(ctx->op == OP_RSH) {
		if (isSignedAndNegative(ctx, ctx->buffer)) return;

		if (ctx->buffer >= ctx->bits) {
			if (!isSignedAndNegative(ctx, ctx->number)) ctx->number = 0;
			else ctx->number = ALL_ONES;
			return;
		}
		else {
			int isNegative = isSignedAndNegative(ctx, ctx->number);

			for (int i = 0; i < ctx->buffer; i++) {
				ctx->number >>= 1;
				if (isNegative)
					ctx->number |= (1ull << (ctx->bits - 1));
			}
		}
	}

	ctx->buffer = 0;
	ctx->op = OP_NONE;
	truncOverflowing(ctx, &ctx->number);
}

int main() {
	// initialize the context
	Context ctx;
	ctx.number = 0;
	ctx.buffer = 0;
	ctx.base = 10;
	ctx.bits = 32;
	ctx.grouping = 0;
	ctx.op = OP_NONE;
	ctx.isSigned = 1;

	// initial print
	print(&ctx);

	// handling input
	char input;
	do {
		input = getch();
		int shouldPrint = 1;

		// BASES
		if (input == KEY_TO_BIN) ctx.base = 2;
		else if (input == KEY_TO_DEC) ctx.base = 10;
		else if (input == KEY_TO_HEX) ctx.base = 16;

		// SIGN
		else if (input == KEY_SIGN && ctx.isSigned)
			if (ctx.op == OP_NONE) negate(&ctx, &ctx.number);
			else negate(&ctx, &ctx.buffer);

		// SIGNEDNESS
		else if (input == KEY_SIGNEDNESS) ctx.isSigned ^= 1;

		// BACKSPACE
		else if (input == KEY_ERASE) {
			if (ctx.op == OP_NONE) eraseDigit(&ctx, &ctx.number);
			else if (ctx.buffer != 0) eraseDigit(&ctx, &ctx.buffer);
			else ctx.op = OP_NONE;
		}

		// CLEAR
		else if (input == KEY_CLEAR) {
			ctx.number = 0;
			ctx.buffer = 0;
			ctx.op = OP_NONE;
		}

		// DIGIT GROUPING
		else if (input == KEY_GROUPING) ctx.grouping ^= 1;

		// BITS
		else if (input == KEY_TO_BYTE && ctx.op == OP_NONE) setBits(&ctx, 8, 0);
		else if (input == KEY_TO_WORD && ctx.op == OP_NONE) setBits(&ctx, 16, 0);
		else if (input == KEY_TO_DWORD && ctx.op == OP_NONE) setBits(&ctx, 32, 0);
		else if (input == KEY_TO_QWORD && ctx.op == OP_NONE) setBits(&ctx, 64, 0);
		else if (input == KEY_TO_BYTE_PS && ctx.op == OP_NONE) setBits(&ctx, 8, 1);
		else if (input == KEY_TO_WORD_PS && ctx.op == OP_NONE) setBits(&ctx, 16, 1);
		else if (input == KEY_TO_DWORD_PS && ctx.op == OP_NONE) setBits(&ctx, 32, 1);
		else if (input == KEY_TO_QWORD_PS && ctx.op == OP_NONE) setBits(&ctx, 64, 1);

		// COPY NUMBER TO BUFFER
		else if (input == KEY_COPY_BUFFER) ctx.buffer = ctx.number;

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
		else if (
			(ctx.base >= 2 && '0' <= input && input <= '1') ||
			(ctx.base >= 10 && '0' <= input && input <= '9') ||
			(ctx.base == 16 && 'a' <= input && input <= 'f')
		) {
			int value = digit2Value(input);
			if (ctx.op == OP_NONE) addDigit(&ctx, &ctx.number, value);
			else addDigit(&ctx, &ctx.buffer, value);
		}

		else shouldPrint = 0; 
		if (shouldPrint) print(&ctx);

	} while (input != KEY_EXIT);

	return 0;
}