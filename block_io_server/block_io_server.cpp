//============================================================================
// Name        : block_io.cpp
// Author      : pangliang
// Version     :
// Copyright   : GPL
// Description : Hello World in C, Ansi-style
//============================================================================

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_PORT	6666
#define BUFFER_SIZE 1024

void * loop(void *arg) {
	int socket = *((int *)arg);
	char buffer[BUFFER_SIZE];
	int len;
	int count = 0;
	while (1) {
		bzero(buffer, BUFFER_SIZE);
		if ((len = recv(socket, buffer, BUFFER_SIZE, 0)) <= 0) {
			printf("Receive Data Failed!\n");
			break;
		}
		if (send(socket, buffer, len, 0) <= 0) {
			printf("Send Failed:%s\n", buffer);
			break;
		}
		
	}
	close (socket);
	return NULL;
}

int main(int argc, char **argv) {
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);

	int server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		printf("Create Socket Failed!");
		exit(1);
	}
	{
		int opt = 1;
		setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	}

	if (bind(server_socket, (struct sockaddr*) &server_addr,
			sizeof(server_addr))) {
		printf("Server Bind Port : %d Failed!", SERVER_PORT);
		exit(1);
	}

	if (listen(server_socket, 20)) {
		printf("Server Listen Failed!");
		exit(1);
	}

	int conn_count = 0;

	while (1) {
		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);

		int new_server_socket = accept(server_socket,
				(struct sockaddr*) &client_addr, &length);
		if (new_server_socket < 0) {
			printf("Server Accept Failed!\n");
			break;
		}

		conn_count++;
		printf("conn:%d\n",conn_count);

		pthread_t tid;

		if (pthread_create(&tid, NULL, loop, (void *) &new_server_socket)
				!= 0) {
			printf("create thread fail");
			break;
		}

	}
	return 0;
}

