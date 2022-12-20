all:
	gcc pipeclient.c -o pipeclient
	gcc pipeserver.c -o pipeserver
	gcc ShmP1.c -o ShmP1
	gcc ShmP2.c -o ShmP2
	gcc sockserver.c -o sockserver
	gcc sockclient.c -o sockclient

IPCpipe:
	gcc pipeclient.c -o pipeclient
	gcc pipeserver.c -o pipeserver
	./pipeclient & ./pipeserver

IPCshm:
	gcc ShmP1.c -o ShmP1
	gcc ShmP2.c -o ShmP2
	./ShmP1 & ./ShmP2

IPCsocket:
	gcc sockserver.c -o sockserver
	gcc sockclient.c -o sockclient
	./sockserver & ./sockclient

clean:
	rm pipeclient pipeserver ShmP1 ShmP2 sockclient sockserver my_fifo