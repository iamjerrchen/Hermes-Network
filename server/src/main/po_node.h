#ifndef P2P_MAIN_PO_NODE_H_
#define P2P_MAIN_PO_NODE_H_

#include "socket.h"

#include <thread>

class PO_Node {

private:
	// constants
	static const unsigned int MAX_NEIGHBORS = 3;
	static const unsigned int CLIENT_PORT = 80085;
	static const unsigned int SERVER_PORT = 43110;

	// private variables
	unsigned long neighbor_ips[MAX_NEIGHBORS];
	std::thread *neighbor_threads[MAX_NEIGHBORS];
	bool filled[MAX_NEIGHBORS]; // determines which neighbors are filled
	
	Socket *client_connection;
	Socket neighbor_listener; // binds to a specified port, listens for connections
	// Socket node_clients[MAX_NEIGHBORS];

	// private functions
	void server_listen();
	bool spawn_server_listener(unsigned int thread_idx);
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