//============================================================================
// Name        : block_io_client.cpp
// Author      : pangliang
// Version     :
// Copyright   : GPL
// Description : Hello World in C, Ansi-style
//============================================================================

#include <netinet/in.h>
#include <arpa/inet.h>
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
	int socket = *((int *) arg);
	char buffer[BUFFER_SIZE];
	int len;
	char *text = (char *)"hello";
	while (1) {

		if (send(socket, text, 5, 0) < 0) {
			printf("Send Failed:%s\n", text);
			break;
		}

		bzero(buffer, BUFFER_SIZE);
		if ((len = recv(socket, buffer, BUFFER_SIZE, 0)) < 0) {
			printf("Receive Data Failed!\n");
			break;
		}
	}
	close(socket);
	return NULL;
}

int main(int argc, char **argv) {

	int conn_count = 0;

	while (1) {
		struct sockaddr_in server_addr;
		bzero(&server_addr, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		server_addr.sin_port = htons(SERVER_PORT);

		int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
		if (socket_fd < 0) {
			printf("Create Socket Failed!");
			exit(1);
		}

		if (connect(socket_fd, (struct sockaddr*) &server_addr,
				sizeof(server_addr))) {
			printf("connect failed!");
			exit(1);
		}

		conn_count++;
		printf("conn:%d\n", conn_count);
		pthread_t tid;
		if (pthread_create(&tid, NULL, loop, (void *) &socket) != 0) {
			printf("create thread fail");
			break;
		}
	}

	return 0;
}
