all:
	gcc Q1_1.c -o Q1_1
	gcc Q1_2.c -o Q1_2
	gcc Q1_1_nosem.c -o Q1_1_nosem

Aone:
	echo making part a with semaphores
	gcc Q1_1.c -o Q1_1
	./Q1_1
	echo part a complete

Atwo:
	echo making part a without semaphores
	gcc Q1_1_nosem.c -o Q1_1_nosem
	./Q1_1_nosem
	echo part a without semaphore complete

Bone:
	echo making part b with semaphores
	gcc Q1_2.c -o Q1_2
	./Q1_2
	echo part b complete

Btwo:
	echo making part b without semaphores
	gcc Q1_2_nosem.c -o Q1_2_nosem
	./Q1_2_nosem
	echo part b without semaphore complete

clean: 
	rm Q1_1 Q1_2 Q1_1_nosem
	echo directory cleaned
