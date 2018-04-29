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
	// std::thread *neighbor_threads[MAX_NEIGHBORS];
	bool filled[MAX_NEIGHBORS]; // determines which neighbors are filled
	// int neighbor_sock_fd[MAX_NEIGHBORS];
	
	Socket *client_connection;
	Socket neighbor_server_sock; // binds to a specified port, listens for connections
	Socket server_neighbor_sock; // Resuable: creates fd for communication


	// private functions
	bool spawn_server_listener(unsigned int thread_idx);
	bool spawn_client_listener();

	bool add_neighbor_ip(unsigned long ip_val);
	// TODO: remove_neighbor_ip() by internal idx
	bool remove_neighbor_ip(unsigned long ip_check);

public:

	PO_Node();

	// Big Three
	~PO_Node();
	PO_Node(const PO_Node &other);
	PO_Node & operator=(const PO_Node &rhs);

	bool add_neighbor_to_server_conn();
	bool add_server_to_neighbor_conn(unsigned long neighbor_ip);
	bool add_neighbor_ip(char *ip); // wrapper
	bool remove_neighbor_ip(char *ip);

};

#endif