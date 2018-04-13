#ifndef P2P_SOCKET_H_
#define P2P_SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>

class Socket {

private:

	static const int port = 8080; // consider making global in namespace
	struct sockaddr_in address;
	int socket_fd; // -1 : undefined

public:

	Socket();
	int create();
	int attach();
	int listen_conn();
	int accept_conn();

};

#endif
