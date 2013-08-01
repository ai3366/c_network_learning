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

class Session{
public:
	int socket;
};

void * loop(void *arg) {
	Session *s = (Session *) arg;
	int socket = s->socket;
	delete s;
	char buffer[BUFFER_SIZE];
	int len;
	char *text = (char *)"hello";
	while (1) {

		if (send(socket, text, 5, 0) <= 0) {
			printf("Send Failed:%s\n", text);
			break;
		}

		bzero(buffer, BUFFER_SIZE);
		if ((len = recv(socket, buffer, BUFFER_SIZE, 0)) <= 0) {
			printf("Receive Data Failed!\n");
			break;
		}

		sleep(1);
	}
	close(socket);
	return NULL;
}

int main(int argc, char **argv) {

	int conn_count = 0;

	printf("connect %s\n",argv[1]);
	
	while (1) {
		struct sockaddr_in server_addr;
		bzero(&server_addr, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = inet_addr(argv[1]);
		server_addr.sin_port = htons(SERVER_PORT);

		Session *s = new Session();
		s->socket = socket(PF_INET, SOCK_STREAM, 0);
		if (s->socket < 0) {
			printf("Create Socket Failed!");
			exit(1);
		}

		if (connect(s->socket, (struct sockaddr*) &server_addr,
				sizeof(server_addr))) {
			printf("connect failed!");
			exit(1);
		}

		conn_count++;
		printf("conn:%d\n", conn_count);
		pthread_t tid;
		if (pthread_create(&tid, NULL, loop, (void *) s) != 0) {
			printf("create thread fail");
			break;
		}

		//break;
	}

	return 0;
}
