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
	~Socket();

	// Prepare socket to accept connections
	int setup_server_socket(int port);

	// create the socket
	int create();
	// Attach the socket to a port
	int attach(int port);
	// Listen on the socket
	int listen_port();
	// Accept incoming connection
	int accept_conn();

};

#endif
