#include "socket.h"

#include <stdio.h>
#include <stdlib.h>

Socket::Socket()
{
	(this->address).sin_family = AF_INET;
	(this->address).sin_addr.s_addr = INADDR_ANY;
	(this->address).sin_port = htons(this->port);
	this->socket_fd = -1;
}

int Socket::create()
{
	// create socket file descriptor
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(this->socket_fd == 0)
	{
		perror("Failed to create server socket.");
		return 0;
	}

	return 1;
}

int Socket::attach()
{
	int opt = 1;
	int ret_check;

	// setting socket options
	ret_check = setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR /*| SO_RESUSEPORT*/, &opt, sizeof(opt));
	if(ret_check)
	{
		perror("Failed to set socket options.");
		return 0;
	}

	ret_check = bind(this->socket_fd, (struct sockaddr*) &this->address, sizeof(address));
	if(ret_check < 0)
	{
		perror("Failed to bind to port.");
		return 0;
	}

	return 1;
}

int Socket::listen_conn()
{
	int ret_check;
	// 3 : max backlog of connections
	ret_check = listen(this->socket_fd, 3);
	if(ret_check < 0)
	{
		perror("Failed to listen on port.");
		return 0;
	}

	return 1;
}

int Socket::accept_conn()
{
	int sock, addrlen;
	addrlen = sizeof(this->address);
	sock = accept(this->socket_fd, (struct sockaddr*) &this->address, (socklen_t*) &addrlen);
	if(sock < 0)
	{
		perror("Failed to accept sonnection.");
	}

	return sock;
}