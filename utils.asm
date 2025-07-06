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
