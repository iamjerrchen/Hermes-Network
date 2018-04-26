#ifndef P2P_MAIN_PO_NODE_H_
#define P2P_MAIN_PO_NODE_H_

#include <thread>
#include "socket.h"

class PO_Node {

private:

	// private variables
	static const unsigned int MAX_NEIGHBORS = 3;
	static const unsigned int CLIENT_PORT = 80085;

	unsigned long neighbor_ips[MAX_NEIGHBORS];
	bool filled[MAX_NEIGHBORS]; // determines which neighbors are filled
	Socket *client_connection;


	// private functions
	bool spawn_listen_thread();
	bool spawn_client_listener();

public:

	PO_Node();

	// Big Three
	~PO_Node();
	PO_Node(const PO_Node &other);
	PO_Node & operator=(const PO_Node &rhs);

	bool add_neighbor(char *ip);
	bool remove_neighbor(char *ip);

};

#endif