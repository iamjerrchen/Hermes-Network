#ifndef P2P_SOCKET_H_
#define P2P_SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>

class Socket {

private:

	struct sockaddr_in address;
	int socket_fd; // -1 : undefined

public:

	Socket();

	// create the socket
	int create();

	/* Server TCP socket methods */
	// Attach the socket to a port
	int attach(int port);
	// Listen on the socket
	int listen_port();
	// Accept incoming connection
	int accept_conn();

};

#endif
