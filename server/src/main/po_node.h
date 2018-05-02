#ifndef P2P_MAIN_PO_NODE_H_
#define P2P_MAIN_PO_NODE_H_

#include "global_data.h"
#include "socket.h"

#include <thread>

// constants
static const unsigned int CLIENT_PORT = 80085; // port to communicate to user
static const unsigned int SERVER_PORT = 43110; // port we bind to
static const unsigned int MAX_NEIGHBORS = 3;



void start_neighbor_to_server_conn(std::string ip, int sock_fd, global_data* data);
bool start_server_to_neighbor_conn(std::string ip, global_data* data);


#endif