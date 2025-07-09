#include <stdint.h>

extern void addBB(uint64_t* io_operand1, uint64_t operand2);
extern void addWW(uint64_t* io_operand1, uint64_t operand2);
extern void addDD(uint64_t* io_operand1, uint64_t operand2);
extern void addQQ(uint64_t* io_operand1, uint64_t operand2);

extern void subBB(uint64_t* io_operand1, uint64_t operand2);
extern void subWW(uint64_t* io_operand1, uint64_t operand2);
extern void subDD(uint64_t* io_operand1, uint64_t operand2);
extern void subQQ(uint64_t* io_operand1, uint64_t operand2);

extern void mulBB(uint64_t* io_lo, uint64_t* o_hi, uint64_t n, uint8_t isSigned);
extern void mulWW(uint64_t* io_lo, uint64_t* o_hi, uint64_t n, uint8_t isSigned);
extern void mulDD(uint64_t* io_lo, uint64_t* o_hi, uint64_t n, uint8_t isSigned);
extern void mulQQ(uint64_t* io_lo, uint64_t* o_hi, uint64_t n, uint8_t isSigned);

extern void divBB(uint64_t* io_lo, uint64_t* io_hi, uint64_t n, uint8_t isSigned);
extern void divWW(uint64_t* io_lo, uint64_t* io_hi, uint64_t n, uint8_t isSigned);
extern void divDD(uint64_t* io_lo, uint64_t* io_hi, uint64_t n, uint8_t isSigned);
extern void divQQ(uint64_t* io_lo, uint64_t* io_hi, uint64_t n, uint8_t isSigned);

extern void orBB(uint64_t* io_operand1, uint64_t operand2);
extern void orWW(uint64_t* io_operand1, uint64_t operand2);
extern void orDD(uint64_t* io_operand1, uint64_t operand2);
extern void orQQ(uint64_t* io_operand1, uint64_t operand2);

extern void andBB(uint64_t* io_operand1, uint64_t operand2);
extern void andWW(uint64_t* io_operand1, uint64_t operand2);
extern void andDD(uint64_t* io_operand1, uint64_t operand2);
extern void andQQ(uint64_t* io_operand1, uint64_t operand2);

extern void xorBB(uint64_t* io_operand1, uint64_t operand2);
extern void xorWW(uint64_t* io_operand1, uint64_t operand2);
extern void xorDD(uint64_t* io_operand1, uint64_t operand2);
extern void xorQQ(uint64_t* io_operand1, uint64_t operand2);

extern void shlBB(uint64_t* io_operand1, uint64_t n);
extern void shlWW(uint64_t* io_operand1, uint64_t n);
extern void shlDD(uint64_t* io_operand1, uint64_t n);
extern void shlQQ(uint64_t* io_operand1, uint64_t n);

extern void shrBB(uint64_t* io_operand1, uint64_t n);
extern void shrWW(uint64_t* io_operand1, uint64_t n);
extern void shrDD(uint64_t* io_operand1, uint64_t n);
extern void shrQQ(uint64_t* io_operand1, uint64_t n);

extern void sarBB(uint64_t* io_operand1, uint64_t n);
extern void sarWW(uint64_t* io_operand1, uint64_t n);
extern void sarDD(uint64_t* io_operand1, uint64_t n);
extern void sarQQ(uint64_t* io_operand1, uint64_t n);


void intmath_add(uint64_t* io_operand1, uint64_t operand2, uint8_t bits) {
	if (bits == 8)
		addBB(io_operand1, operand2);
	else if (bits == 16)
		addWW(io_operand1, operand2);
	else if (bits == 32)
		addDD(io_operand1, operand2);
	else
		addQQ(io_operand1, operand2);
}

void intmath_sub(uint64_t* io_operand1, uint64_t operand2, uint8_t bits) {
	if (bits == 8)
		subBB(io_operand1, operand2);
	else if (bits == 16)
		subWW(io_operand1, operand2);
	else if (bits == 32)
		subDD(io_operand1, operand2);
	else
		subQQ(io_operand1, operand2);
}

void intmath_mul(uint64_t* io_lo, uint64_t* o_hi, uint64_t n, uint8_t isSigned, uint8_t bits) {
	if (bits == 8)
		mulBB(io_lo, o_hi, n, isSigned);
	else if (bits == 16)
		mulWW(io_lo, o_hi, n, isSigned);
	else if (bits == 32)
		mulDD(io_lo, o_hi, n, isSigned);
	else
		mulQQ(io_lo, o_hi, n, isSigned);
}

void intmath_div(uint64_t* io_lo, uint64_t* io_hi, uint64_t n, uint8_t isSigned, uint8_t bits) {
	if (bits == 8)
		divBB(io_lo, io_hi, n, isSigned);
	else if (bits == 16)
		divWW(io_lo, io_hi, n, isSigned);
	else if (bits == 32)
		divDD(io_lo, io_hi, n, isSigned);
	else
		divQQ(io_lo, io_hi, n, isSigned);
}

void intmath_and(uint64_t* io_operand1, uint64_t operand2, uint8_t bits) {
	if (bits == 8)
		andBB(io_operand1, operand2);
	else if (bits == 16)
		andWW(io_operand1, operand2);
	else if (bits == 32)
		andDD(io_operand1, operand2);
	else
		andQQ(io_operand1, operand2);
}

void intmath_or(uint64_t* io_operand1, uint64_t operand2, uint8_t bits) {
	if (bits == 8)
		orBB(io_operand1, operand2);
	else if (bits == 16)
		orWW(io_operand1, operand2);
	else if (bits == 32)
		orDD(io_operand1, operand2);
	else
		orQQ(io_operand1, operand2);
}

void intmath_xor(uint64_t* io_operand1, uint64_t operand2, uint8_t bits) {
	if (bits == 8)
		xorBB(io_operand1, operand2);
	else if (bits == 16)
		xorWW(io_operand1, operand2);
	else if (bits == 32)
		xorDD(io_operand1, operand2);
	else
		xorQQ(io_operand1, operand2);
}

void intmath_lsh(uint64_t* io_operand1, uint64_t operand2, uint8_t bits) {
	if (bits == 8)
		shlBB(io_operand1, operand2);
	else if (bits == 16)
		shlWW(io_operand1, operand2);
	else if (bits == 32)
		shlDD(io_operand1, operand2);
	else
		shlQQ(io_operand1, operand2);
}

void intmath_rsh(uint64_t* io_operand1, uint64_t operand2, uint8_t isSigned, uint8_t bits) {
	if (!isSigned) {
		if (bits == 8)
			shrBB(io_operand1, operand2);
		else if (bits == 16)
			shrWW(io_operand1, operand2);
		else if (bits == 32)
			shrDD(io_operand1, operand2);
		else
			shrQQ(io_operand1, operand2);
	} else {
		if (bits == 8)
			sarBB(io_operand1, operand2);
		else if (bits == 16)
			sarWW(io_operand1, operand2);
		else if (bits == 32)
			sarDD(io_operand1, operand2);
		else
			sarQQ(io_operand1, operand2);
	}
}
