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
#include <errno.h>

#define SERVER_PORT	6666
#define BUFFER_SIZE 1024

struct socket_h {
	int socket;
};

void * loop(void *arg) {
	socket_h *socket_fd = (socket_h *)arg;
	int socket=socket_fd->socket;
	free(arg);
	char buffer[BUFFER_SIZE];
	int len;
	char *text = (char *) "hello";
	//int count=0;
	int ret = 0;
	while (1) {

		if ((ret = send(socket, text, 5, 0)) <= 0) {
			printf("Send Failed, ret:%d, socket:%d\n", ret,socket);
			break;
		}

		bzero(buffer, BUFFER_SIZE);
		if ((len = recv(socket, buffer, BUFFER_SIZE, 0)) <= 0) {
			printf("Receive Data Failed!\n");
			break;
		}
		//printf("socket alive:%d\n", socket);
		sleep(1);
	}

	if (close(socket) != 0)
		printf("close socket %d error\n", socket);
	return NULL;
}


int main(int argc, char **argv) {

	int conn_count = 0;

	printf("connect %s\n", argv[1]);
	while (1) {
		struct sockaddr_in server_addr;
		bzero(&server_addr, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = inet_addr(argv[1]);
		server_addr.sin_port = htons(SERVER_PORT);

		printf("create socket...\n");

		socket_h * sh=(socket_h *)malloc(sizeof(socket_h));
		sh->socket = socket(PF_INET, SOCK_STREAM, 0);
		if (sh->socket < 0) {
			printf("Create Socket Failed!");
			exit(1);
		}

		printf("connect...use socket:%d,%p\n",sh->socket,sh);
		if (connect(sh->socket, (struct sockaddr*) &server_addr,
				sizeof(server_addr)) != 0) {
			printf("connect failed!");
			if (errno != EINPROGRESS) {
				perror("connect");
				break;
			}
		}

		pthread_t tid;
		if (pthread_create(&tid, NULL, loop, (void *) sh) != 0) {
			printf("create thread fail");
			break;
		}

		conn_count++;
		printf("new tid:%ld, conn:%d\n", (unsigned long) tid, conn_count);

		//sleep(1);
		//break;
	}

	return 0;
}
