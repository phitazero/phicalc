section .text


global addBB
addBB:                  ; passed: &mainReg, inputReg
	add [rdi], sil      ; mainReg += inputReg
	ret

global addWW
addWW:                  ; passed: &mainReg, inputReg
	add [rdi], si       ; mainReg += inputReg
	ret

global addDD
addDD:                  ; passed: &mainReg, inputReg
	add [rdi], esi      ; mainReg += inputReg
	ret

global addQQ
addQQ:                  ; passed: &mainReg, inputReg
	add [rdi], rsi      ; mainReg += inputReg
	ret



global subBB
subBB:                  ; passed: &mainReg, inputReg
	sub [rdi], sil      ; mainReg -= inputReg
	ret

global subWW
subWW:                  ; passed: &mainReg, inputReg
	sub [rdi], si       ; mainReg -= inputReg
	ret

global subDD
subDD:                  ; passed: &mainReg, inputReg
	sub [rdi], esi      ; mainReg -= inputReg
	ret

global subQQ
subQQ:                  ; passed: &mainReg, inputReg
	sub [rdi], rsi      ; mainReg -= inputReg
	ret




global mulBB
mulBB:                  ; passed: &lo, &hi, n, isSigned
	mov al, [rdi]       ; al = lo

	; ex:ax = ax * n
	test cl, cl
	jnz .l0             ; if isSigned do imul
	mul dl              ; else do mul
	jmp .l1
	.l0:
	imul dl
	.l1:

	mov [rdi], al       ; lo = al
	mov [rsi], ah       ; hi = ah
	ret

global mulWW
mulWW:                  ; passed: &lo, &hi, n, isSigned
	mov ax, [rdi]       ; ax = lo

	; ex:ax = ax * n
	test cl, cl
	jnz .l0             ; if isSigned do imul
	mul dx              ; else do mul
	jmp .l1
	.l0:
	imul dx
	.l1:

	mov [rdi], ax       ; lo = ax
	mov [rsi], dx       ; hi = dx
	ret

global mulDD
mulDD:                  ; passed: &lo, &hi, n, isSigned
	mov eax, [rdi]      ; eax = lo

	; edx:eax = eax * n
	test cl, cl
	jnz .l0             ; if isSigned do imul
	mul edx             ; else do mul
	jmp .l1
	.l0:
	imul edx
	.l1:

	mov [rdi], eax      ; lo = eax
	mov [rsi], edx      ; hi = edx
	ret


global mulQQ
mulQQ:                  ; passed: &lo, &hi, n, isSigned
	mov rax, [rdi]      ; rax = lo

	; rdx:rax = rax * n
	test cl, cl
	jnz .l0             ; if isSigned do imul
	mul rdx             ; else do mul
	jmp .l1
	.l0:
	imul rdx
	.l1:

	mov [rdi], rax      ; lo = rax
	mov [rsi], rdx      ; hi = rdx
	ret



global divBB
divBB:                  ; passed: &lo, &hi, n, isSigned
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

	mov [rdi], al       ; lo = al (quotient)
	mov [rsi], ah       ; hi = ah (remainder)
	ret

global divWW
divWW:                  ; passed: &lo, &hi, n, isSigned
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

	mov [rdi], ax       ; lo = ax (quotient)
	mov [rsi], dx       ; hi = dx (remainder)
	ret

global divDD
divDD:                  ; passed: &lo, &hi, n, isSigned
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

	mov [rdi], eax      ; lo = eax (quotient)
	mov [rsi], edx      ; hi = edx (remainder)
	ret

global divQQ
divQQ:                  ; passed: &lo, &hi, n, isSigned
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

	mov [rdi], rax      ; lo = rax (quotient)
	mov [rsi], rdx      ; hi = rdx (remainder)
	ret



global andBB
andBB:                  ; passed: &mainReg, inputReg
	and [rdi], sil      ; mainReg &= inputReg
	ret

global andWW
andWW:                  ; passed: &mainReg, inputReg
	and [rdi], si       ; mainReg &= inputReg
	ret

global andDD
andDD:                  ; passed: &mainReg, inputReg
	and [rdi], esi      ; mainReg &= inputReg
	ret

global andQQ
andQQ:                  ; passed: &mainReg, inputReg
	and [rdi], rsi      ; mainReg &= inputReg
	ret



global orBB
orBB:                   ; passed: &mainReg, inputReg
	or [rdi], sil       ; mainReg |= inputReg
	ret

global orWW
orWW:                   ; passed: &mainReg, inputReg
	or [rdi], si        ; mainReg |= inputReg
	ret

global orDD
orDD:                   ; passed: &mainReg, inputReg
	or [rdi], esi       ; mainReg |= inputReg
	ret

global orQQ
orQQ:                   ; passed: &mainReg, inputReg
	or [rdi], rsi       ; mainReg |= inputReg
	ret



global xorBB
xorBB:                  ; passed: &mainReg, inputReg
	xor [rdi], sil      ; mainReg ^= inputReg
	ret

global xorWW
xorWW:                  ; passed: &mainReg, inputReg
	xor [rdi], si       ; mainReg ^= inputReg
	ret

global xorDD
xorDD:                  ; passed: &mainReg, inputReg
	xor [rdi], esi      ; mainReg ^= inputReg
	ret

global xorQQ
xorQQ:                  ; passed: &mainReg, inputReg
	xor [rdi], rsi      ; mainReg ^= inputReg
	ret



global shlBB
shlBB:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	shl byte [rdi], cl  ; mainReg <<= n
	ret

global shlWW
shlWW:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	shl word [rdi], cl  ; mainReg <<= n
	ret

global shlDD
shlDD:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	shl dword [rdi], cl ; mainReg <<= n
	ret

global shlQQ
shlQQ:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	shl qword [rdi], cl ; mainReg <<= n
	ret




global shrBB
shrBB:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	shr byte [rdi], cl  ; mainReg >>= n (logical)
	ret

global shrWW
shrWW:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	shr word [rdi], cl  ; mainReg >>= n (logical)
	ret

global shrDD
shrDD:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	shr dword [rdi], cl ; mainReg >>= n (logical)
	ret

global shrQQ
shrQQ:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	shr qword [rdi], cl ; mainReg >>= n (logical)
	ret



global sarBB
sarBB:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	sar byte [rdi], cl  ; mainReg >>= n (arithmetic)
	ret

global sarWW
sarWW:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	sar word [rdi], cl  ; mainReg >>= n (arithmetic)
	ret

global sarDD
sarDD:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	sar dword [rdi], cl ; mainReg >>= n (arithmetic)
	ret

global sarQQ
sarQQ:                  ; passed: &mainReg, n
	mov cl, sil         ; cl = n
	sar qword [rdi], cl ; mainReg >>= n (arithmetic)
	ret
