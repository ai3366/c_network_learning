#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_PORT	6666
#define BUFFER_SIZE 1024
#define	MAX_EVENTS 64

int create_socket(int port) {
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(port);

	int server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		return -1;
	}

	if (bind(server_socket, (struct sockaddr*) &server_addr,
				sizeof(server_addr))) {
		return -1;
	}
	return server_socket;
}

int set_socket_non_blocking(int sfd)
{
	int flags = fcntl(sfd,F_GETFL,0);
	if(-1 == flags)
		return -1;
	flags |= O_NONBLOCK;
	if(-1 == fcntl(sfd,F_SETFL,flags))
		return -1;
	return 0;
}

int accept_handle(int efd, int server_socket)
{
	struct sockaddr in_addr;
	socklen_t in_addr_len =(socklen_t) sizeof(in_addr);
	int client_socket = accept(server_socket, &in_addr, &in_addr_len);
	if(client_socket < 0)
	{
		printf("accept error");
		return -1;
	}
	if(set_socket_non_blocking(client_socket))
	{
		perror("set non blocking error");
		return -1;
	}
	struct epoll_event event;
	event.data.fd = client_socket;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(efd,EPOLL_CTL_ADD, client_socket,&event);

	return 0;

}

void read_handle(int socket)
{
	char buf[BUFFER_SIZE];
	memset(buf,0,BUFFER_SIZE);
	int count = read(socket,buf,BUFFER_SIZE);
	if(count > 0)
		printf("recv: %s\n",buf);
}

void server_event_start(int socket)
{
	int efd = epoll_create1 (0);
	struct epoll_event event;
	event.data.fd = socket;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(efd,EPOLL_CTL_ADD, socket,&event);

	struct epoll_event *recv_events;
	recv_events = (epoll_event*) calloc(MAX_EVENTS , sizeof(epoll_event));

	while(true)
	{
		int n = epoll_wait(efd, recv_events, MAX_EVENTS, -1);
		printf("epoll_wait return %d\n",n);
		for(int i = 0; i<n; i++)
		{
			if(socket == recv_events[i].data.fd)
			{
				accept_handle(efd,socket);
			}else{
				read_handle(recv_events[i].data.fd);
			}
		}
	}

}

int main(int argc, char *argv[])
{
	int server_socket = create_socket(SERVER_PORT);
	if(server_socket < 0 )
	{
		perror("create socket error");
		exit(1);
	}
	if(set_socket_non_blocking(server_socket))
	{
		perror("set non blocking error");
		exit(1);
	}
	if (listen(server_socket, 20)) {
		printf("Server Listen Failed!");
		exit(1);
	}

	server_event_start(server_socket);

	close(server_socket);
	
	return 0;
}

