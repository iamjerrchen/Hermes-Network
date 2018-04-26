#ifndef P2P_MAIN_SOCKET_H_
#define P2P_MAIN_SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>

class Socket {

private:

	struct sockaddr_in address;
	int port; // definable by caller
	int socket_fd; // -1 : undefined

	int max_queued_conn; // maximum queued connections

public:

	Socket();
	~Socket();

	// Prepare socket to accept connections
	bool setup_server_socket(int port);

	// create the socket
	bool create();
	// Attach the socket to a port
	bool attach(int port);
	// Listen on the socket
	bool listen_port();
	// Accept incoming connection
	int accept_conn();

};

#endif
