all:
	yasm hello.s -f elf64 -o hello.o && gcc -Wall -Weaxtra -Werror - no-pie -o hello hello.o
	./hello