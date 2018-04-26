#ifndef P2P_MAIN_PO_NODE_H_
#define P2P_MAIN_PO_NODE_H_

#include <thread>

class PO_Node {

private:

	// private variables
	static const unsigned int max_neighbors = 3;
	unsigned long *neighbor_ips;
	bool *filled; // determines which neighbors are filled

	// private functions
	bool spawn_listen_thread();

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