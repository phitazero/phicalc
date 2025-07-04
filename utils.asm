section .text
global mul128bHiPart

mul128bHiPart:
	mov rax, [rdi]
	mul rsi
	mov [rdi], rax
	mov rax, rdx
	ret
