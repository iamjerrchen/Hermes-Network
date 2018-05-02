#ifndef P2P_MAIN_PO_NODE_H_
#define P2P_MAIN_PO_NODE_H_

#include "socket.h"

#include <thread>

// constants
static const unsigned int CLIENT_PORT = 80085; // port to communicate to user
static const unsigned int SERVER_PORT = 43110; // port we bind to
static const unsigned int MAX_NEIGHBORS = 3;

// private variables
unsigned long neighbor_ips[MAX_NEIGHBORS];
bool filled[MAX_NEIGHBORS]; // determines which neighbors are filled


Socket *client_connection;
Socket neighbor_server_sock; // binds to a specified port, listens for connections
Socket server_neighbor_sock; // Resuable: creates fd for communication


void start_neighbor_to_server_conn(string ip, int sock_fd);
bool start_server_to_neighbor_conn(string ip);

//bool add_neighbor_ip(char *ip); // wrapper
//bool remove_neighbor_ip(char *ip);


#endif