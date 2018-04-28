/* socket.cpp
 *
 *
 *
 */

#include "socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
// system logging
#include <string.h>
#include <syslog.h>
#include <errno.h>

Socket::Socket()
{
	memset(&(this->address), '0', sizeof(this->address));
	this->port = -1;
	this->socket_fd = -1;
	// TODO: Consider other max backlog values
	this->max_queued_conn = 5;
}

Socket::~Socket()
{
	/* TODO: Properly close socketfd.
	 * 0: stop receiving data
	 * 1: stop transmissing data
	 * 2: stop receiving and transmitting
	 */
	shutdown(this->socket_fd, 2);
}

/* Server TCP socket methods */
bool Socket::setup_server_socket(int port)
{
	int success = 0;
	this->port = port;
	if(create())
	{
		if(attach(port))
		{
			if(listen_port())
			{
				success = 1;
			}
		}
	}
	return success;
}

// port = port to connect to
int Socket::setup_client_socket(int port, char* ip)
{
	int success = 0;
	this->port = port;
	if(create())
	{
		if(configure_address(port, ip))
		{
			if(connect_server())
			{
				return this->socket_fd;				
			}
		}
	}
	return -1;
}

// used by client to connect to server address, similar to accept
bool Socket::connect_server()
{
	if(connect(this->socket_fd, (struct sockaddr *)&(this->address), sizeof(this->address)) < 0)
	{
		syslog(LOG_WARNING, "[socket] Failed to connect to server.");
		return false;
	}
	return true;
}

// used by client to create server address
bool Socket::configure_address(int port, char* ip)
{
	(this->address).sin_family = AF_INET;
	(this->address).sin_port = htons(port);
	if(inet_pton(AF_INET, ip, &(this->address).sin_addr) <= 0)
	{
		syslog(LOG_ERR, "[socket] Can't configure. Invalid address or address not supported: %s", strerror(errno));
		return false;
	}
	return true;
}

// used by both client and server
bool Socket::create()
{
	// create socket file descriptor
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(this->socket_fd == 0)
	{
		syslog(LOG_ERR, "[socket] Failed to create a socket: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}

	return 1;
}

// used by server to bind to a port
bool Socket::attach(int port)
{
	int opt = 1;
	int ret_check;

	// setting socket options
	ret_check = setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	if(ret_check)
	{
		syslog(LOG_ERR, "[socket] Failed to set socket options: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// attach socket to port
	(this->address).sin_family = AF_INET;
	(this->address).sin_addr.s_addr = INADDR_ANY;
	(this->address).sin_port = htons(port);
	ret_check = bind(this->socket_fd, (struct sockaddr*) &(this->address), sizeof(address));
	if(ret_check < 0)
	{
		// Failure to bind to port should return with 0 instead of exit.
		syslog(LOG_ERR, "[socket] Failed to bind to port %d: %s", port, strerror(errno));
		return 0;
	}

	return 1;
}

// used by server to listen on a port
bool Socket::listen_port()
{
	int ret_check;

	ret_check = listen(this->socket_fd, this->max_queued_conn);
	if(ret_check < 0)
	{
		syslog(LOG_ERR, "[socket] Failed to listen on port %d: %s", this->port, strerror(errno));
		exit(EXIT_FAILURE);
	}

	return 1;
}

// used by server to accept incoming connections
int Socket::accept_conn()
{
	int sock, addrlen;

	// accept incoming connection
	addrlen = sizeof(this->address);
	sock = accept(this->socket_fd, (struct sockaddr*) &(this->address), (socklen_t*) &addrlen);
	if(sock < 0)
	{
		syslog(LOG_WARNING, "[socket] Failed to accept connection: %s", strerror(errno));
	}

	return sock;
}
