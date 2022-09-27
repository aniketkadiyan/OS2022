section .data
	msgstr: db "enter a string:", 10 ,0
	formatstr: db "%s", 0
	
section .bss
	string resb 4096
	
section .text
		global main
		extern printf
		extern scanf
	
	main:
		push rbp
		mov rbp, rsp
		sub rsp, 16
		
		mov rdi, msgstr
		mov al,0
		call printf
		
		mov [string], rax
		mov rdi, formatstr
		lea rsi, [string]
		mov al, 0
		call scanf
		
		mov rdi, string
		mov al, 0
		call printf
		
		add rsp, 16
		leave
		ret
		
	