// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	// socket setup done

	int i = 0, maxindex = 0, st = 1;
	while (st == 1)
	{
		// reading 5 elements
		printf("\n server receiving data:\nid --- string\n");
		for (i = 0; i < 5; i++)
		{
			valread = read(new_socket, buffer, 11);
			char *index = malloc(2);
			char *str = malloc(6);
			strncpy(index, buffer, 2);
			strncpy(str, buffer + 5, 6);
			printf("%d --- %s\n", atoi(index), str);
			if (atoi(index) > maxindex)
			{
				maxindex = atoi(index);
			}
			// printf("%s\n", buffer);
		}

		// sending acknowledgement
		char *str = malloc(3);
		if (maxindex < 10)
		{
			snprintf(str, 3, "0%d", maxindex);
		}
		else
		{
			snprintf(str, 3, "%d", maxindex);
		}
		printf("server sending acknowledgement : %s\n",str);
		send(new_socket, str, strlen(str), 0);
		if(maxindex==49){
			st=0;
		}
		sleep(3);
	}

	// valread = read(new_socket, buffer, 17);

	// printf("%s\n", buffer);

	// send(new_socket, hello, strlen(hello), 0);
	// printf("Hello message sent\n");

	printf("\nserver shutdown\n");

	close(new_socket);

	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
