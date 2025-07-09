#include <stdint.h>

extern void mulBB(uint64_t* io_lo, uint64_t* o_hi, uint64_t n, uint8_t isSigned);
extern void mulWW(uint64_t* io_lo, uint64_t* o_hi, uint64_t n, uint8_t isSigned);
extern void mulDD(uint64_t* io_lo, uint64_t* o_hi, uint64_t n, uint8_t isSigned);
extern void mulQQ(uint64_t* io_lo, uint64_t* o_hi, uint64_t n, uint8_t isSigned);

extern void divBB(uint64_t* io_lo, uint64_t* io_hi, uint64_t n, uint8_t isSigned);
extern void divWW(uint64_t* io_lo, uint64_t* io_hi, uint64_t n, uint8_t isSigned);
extern void divDD(uint64_t* io_lo, uint64_t* io_hi, uint64_t n, uint8_t isSigned);
extern void divQQ(uint64_t* io_lo, uint64_t* io_hi, uint64_t n, uint8_t isSigned);

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
