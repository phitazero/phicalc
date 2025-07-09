section .text

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
