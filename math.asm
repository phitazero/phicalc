section .text

dumpFlags:              ; passed: r8: &flags, r9: flagMask, pushf'd flags
	mov r10, [rsp + 8]

	; set all flags to undefined
	mov dword [r8], -1

	test r9, 1
	jz .l0

	bt r10, 7
	setc [r8]

	.l0:
	test r9, 2
	jz .l1

	bt r10, 6
	setc [r8 + 1]

	.l1:
	test r9, 4
	jz .l2

	bt r10, 0
	setc [r8 + 2]

	.l2:
	test r9, 8
	jz .l3

	bt r10, 11
	setc [r8 + 3]

	.l3:
	ret 8

dumpFlagsAfterShift:    ; passed: rdx: &flags, cl: n, pushf'd flags
	mov r9, 0b0111      ; SF, ZF, CF are affected

	; if n == 1 then OF is affected too
	cmp cl, 1
	jne .l0
	or r9, 0b1000

	.l0:
	mov r8, rdx
	push qword [rsp + 8]
	call dumpFlags
	ret 8

global addBB
addBB:                  ; passed: &mainReg, inputReg, &flags
	add [rdi], sil      ; mainReg += inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global addWW
addWW:                  ; passed: &mainReg, inputReg, &flags
	add [rdi], si       ; mainReg += inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global addDD
addDD:                  ; passed: &mainReg, inputReg, &flags
	add [rdi], esi      ; mainReg += inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global addQQ
addQQ:                  ; passed: &mainReg, inputReg, &flags
	add [rdi], rsi      ; mainReg += inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret



global subBB
subBB:                  ; passed: &mainReg, inputReg, &flags
	sub [rdi], sil      ; mainReg -= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global subWW
subWW:                  ; passed: &mainReg, inputReg, &flags
	sub [rdi], si       ; mainReg -= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global subDD
subDD:                  ; passed: &mainReg, inputReg, &flags
	sub [rdi], esi      ; mainReg -= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global subQQ
subQQ:                  ; passed: &mainReg, inputReg, &flags
	sub [rdi], rsi      ; mainReg -= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret




global mulBB
mulBB:                  ; passed: &lo, &hi, n, isSigned, &flags
	mov al, [rdi]       ; al = lo

	; ex:ax = ax * n
	test cl, cl
	jnz .l0             ; if isSigned do imul
	mul dl              ; else do mul
	jmp .l1
	.l0:
	imul dl
	.l1:

	; r8 already points to Flags
	mov r9, 0b1100      ; CF and OF are affected
	pushf
	call dumpFlags

	mov [rdi], al       ; lo = al
	mov [rsi], ah       ; hi = ah
	ret

global mulWW
mulWW:                  ; passed: &lo, &hi, n, isSigned, &flags
	mov ax, [rdi]       ; ax = lo

	; ex:ax = ax * n
	test cl, cl
	jnz .l0             ; if isSigned do imul
	mul dx              ; else do mul
	jmp .l1
	.l0:
	imul dx
	.l1:

	; r8 already points to Flags
	mov r9, 0b1100      ; CF and OF are affected
	pushf
	call dumpFlags

	mov [rdi], ax       ; lo = ax
	mov [rsi], dx       ; hi = dx
	ret

global mulDD
mulDD:                  ; passed: &lo, &hi, n, isSigned, &flags
	mov eax, [rdi]      ; eax = lo

	; edx:eax = eax * n
	test cl, cl
	jnz .l0             ; if isSigned do imul
	mul edx             ; else do mul
	jmp .l1 
	.l0:
	imul edx
	.l1:

	; r8 already points to Flags
	mov r9, 0b1100      ; CF and OF are affected
	pushf
	call dumpFlags

	mov [rdi], eax      ; lo = eax
	mov [rsi], edx      ; hi = edx
	ret


global mulQQ
mulQQ:                  ; passed: &lo, &hi, n, isSigned, &flags
	mov rax, [rdi]      ; rax = lo

	; rdx:rax = rax * n
	test cl, cl
	jnz .l0             ; if isSigned do imul
	mul rdx             ; else do mul
	jmp .l1
	.l0:
	imul rdx
	.l1:

	; r8 already points to Flags
	mov r9, 0b1100      ; CF and OF are affected
	pushf
	call dumpFlags

	mov [rdi], rax      ; lo = rax
	mov [rsi], rdx      ; hi = rdx
	ret



global divBB
divBB:                  ; passed: &lo, &hi, n, isSigned, &flags
	; copying the divisor to r11 isn't neccessary for byte arguments
	; but is kept for consistency

	mov r11b, dl        ; r11b = n
	mov al, [rdi]       ; al = lo

	test cl, cl
	jnz .l0             ; if isSigned do idiv
	xor ah, ah          ; else do div, ah = 0
	div r11b
	jmp .l1
	.l0:
	cbw
	idiv r11b
	.l1:

	; r8 already points to Flags
	xor r9, r9          ; no flags are affected
	pushf
	call dumpFlags

	mov [rdi], al       ; lo = al (quotient)
	mov [rsi], ah       ; hi = ah (remainder)
	ret

global divWW
divWW:                  ; passed: &lo, &hi, n, isSigned, &flags
	mov r11w, dx        ; r11w = n
	mov ax, [rdi]       ; ax = lo

	test cl, cl
	jnz .l0             ; if isSigned do idiv
	xor dx, dx          ; else do div, dx = 0
	div r11w
	jmp .l1
	.l0:
	cwd
	idiv r11w
	.l1:

	; r8 already points to Flags
	xor r9, r9          ; no flags are affected
	pushf
	call dumpFlags

	mov [rdi], ax       ; lo = ax (quotient)
	mov [rsi], dx       ; hi = dx (remainder)
	ret

global divDD
divDD:                  ; passed: &lo, &hi, n, isSigned, &flags
	mov r11d, edx       ; r11d = n
	mov eax, [rdi]      ; eax = lo

	test cl, cl
	jnz .l0             ; if isSigned do idiv
	xor edx, edx        ; else do div, edx = 0
	div r11d
	jmp .l1
	.l0:
	cdq
	idiv r11d
	.l1:

	; r8 already points to Flags
	xor r9, r9          ; no flags are affected
	pushf
	call dumpFlags

	mov [rdi], eax      ; lo = eax (quotient)
	mov [rsi], edx      ; hi = edx (remainder)
	ret

global divQQ
divQQ:                  ; passed: &lo, &hi, n, isSigned, &flags
	mov r11, rdx        ; r11 = n
	mov rax, [rdi]      ; rax = lo

	test cl, cl
	jnz .l0             ; if isSigned do idiv
	xor rdx, rdx        ; else do div, rdx = 0
	div r11
	jmp .l1
	.l0:
	cqo
	idiv r11
	.l1:

	; r8 already points to Flags
	xor r9, r9          ; no flags are affected
	pushf
	call dumpFlags

	mov [rdi], rax      ; lo = rax (quotient)
	mov [rsi], rdx      ; hi = rdx (remainder)
	ret



global andBB
andBB:                  ; passed: &mainReg, inputReg, &flags
	and [rdi], sil      ; mainReg &= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global andWW
andWW:                  ; passed: &mainReg, inputReg, &flags
	and [rdi], si       ; mainReg &= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global andDD
andDD:                  ; passed: &mainReg, inputReg, &flags
	and [rdi], esi      ; mainReg &= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global andQQ
andQQ:                  ; passed: &mainReg, inputReg, &flags
	and [rdi], rsi      ; mainReg &= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret



global orBB
orBB:                   ; passed: &mainReg, inputReg, &flags
	or [rdi], sil       ; mainReg |= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global orWW
orWW:                   ; passed: &mainReg, inputReg, &flags
	or [rdi], si        ; mainReg |= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global orDD
orDD:                   ; passed: &mainReg, inputReg, &flags
	or [rdi], esi       ; mainReg |= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global orQQ
orQQ:                   ; passed: &mainReg, inputReg, &flags
	or [rdi], rsi       ; mainReg |= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret



global xorBB
xorBB:                  ; passed: &mainReg, inputReg, &flags
	xor [rdi], sil      ; mainReg ^= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global xorWW
xorWW:                  ; passed: &mainReg, inputReg, &flags
	xor [rdi], si       ; mainReg ^= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global xorDD
xorDD:                  ; passed: &mainReg, inputReg, &flags
	xor [rdi], esi      ; mainReg ^= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret

global xorQQ
xorQQ:                  ; passed: &mainReg, inputReg, &flags
	xor [rdi], rsi      ; mainReg ^= inputReg

	mov r8, rdx
	mov r9, 0b1111      ; all flags affected
	pushf
	call dumpFlags

	ret



global shlBB
shlBB:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	shl byte [rdi], cl  ; mainReg <<= n
	pushf
	call dumpFlagsAfterShift
	ret

global shlWW
shlWW:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	shl word [rdi], cl  ; mainReg <<= n
	pushf
	call dumpFlagsAfterShift
	ret

global shlDD
shlDD:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	shl dword [rdi], cl ; mainReg <<= n
	pushf
	call dumpFlagsAfterShift
	ret

global shlQQ
shlQQ:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	shl qword [rdi], cl ; mainReg <<= n
	pushf
	call dumpFlagsAfterShift
	ret




global shrBB
shrBB:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	shr byte [rdi], cl  ; mainReg >>= n (logical)
	pushf
	call dumpFlagsAfterShift
	ret

global shrWW
shrWW:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	shr word [rdi], cl  ; mainReg >>= n (logical)
	pushf
	call dumpFlagsAfterShift
	ret

global shrDD
shrDD:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	shr dword [rdi], cl ; mainReg >>= n (logical)
	pushf
	call dumpFlagsAfterShift
	ret

global shrQQ
shrQQ:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	shr qword [rdi], cl ; mainReg >>= n (logical)
	pushf
	call dumpFlagsAfterShift
	ret



global sarBB
sarBB:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	sar byte [rdi], cl  ; mainReg >>= n (arithmetic)
	pushf
	call dumpFlagsAfterShift
	ret

global sarWW
sarWW:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	sar word [rdi], cl  ; mainReg >>= n (arithmetic)
	pushf
	call dumpFlagsAfterShift
	ret

global sarDD
sarDD:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	sar dword [rdi], cl ; mainReg >>= n (arithmetic)
	pushf
	call dumpFlagsAfterShift
	ret

global sarQQ
sarQQ:                  ; passed: &mainReg, n, &flags
	mov cl, sil         ; cl = n
	sar qword [rdi], cl ; mainReg >>= n (arithmetic)
	pushf
	call dumpFlagsAfterShift
	ret
