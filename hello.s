section .data
	msgint: db "enter a number: ", 10, 0
	formatint: db "%d", 0
	msgstr: db "enter a string: ", 10, 0
	formatstr: db "%s", 0
	
section .bss
	number resb 4
	string resw 4
	
section .text
		global main
		extern scanf
		extern printf
		
	main:
		push rbp
		mov rbp, rsp
		sub rsp, 16 ;alignment for 64-bit system
		
		;printing integer input msg
		xor rax, rax ;could use move rax, 1
		lea rdi, [msgint]
		call printf
		
		;taking integer input using scanf
		mov rax, 1
		lea rdi, [formatint]
		lea rsi, [number]
		call scanf
		
		;outputting the entered number
		xor rax, rax
		lea rsi, [number]
		call printf
		
		;printing string input prompt
		xor rax, rax ; redundant but included for sake of uniformity
		lea rdi, [msgstr]
		call printf
		
		; taking string input using scanf
		;this section does not work for some reason
		; scanf wont take string input
		;no online examples available
		
		mov rax, 0
		lea rdi, [formatstr]
		lea rsi , [string]
		call scanf
		
		;this section should print string input
		;but due to previous section not working
		;it prints the integer we entered before
		
		xor rax, rax
		lea rsi [string]
		call printf
		
		;re-alignment back to normal state to avoid segfault
		add rsp, 16
		leave
		ret

		
		
		