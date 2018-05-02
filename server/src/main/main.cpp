#include <stdlib.h>
#include <stdio.h>
#include "global_data.h"
#include "socket.h"
#include "po_node.h"


int main(int argc, char * argv[]) {
	if (argc > 4)
		exit(EXIT_FAILURE);

	global_data globals;
	globals.incoming_messages = new std::map<std::string,std::queue<std::string>>();
	globals.outgoing_messages = new std::map<std::string,std::queue<std::string>>();

	// Create client connection
	
	// Create node connections
	for (int i = 1; i <= argc; i++) {
		std::string ip(argv[i]);
		std::thread new_conn(start_server_to_neighbor_conn, ip, &globals);
		new_conn.detach();
	}
	
	// create server socket
	Socket server_listener;
	while (1) {
		int sock_fd = -1;
		if (globals.num_connections < 3) {
			sock_fd = server_listener.accept_conn();
			std::string ip(server_listener.get_most_recent_ip());
			std::thread new_conn(start_neighbor_to_server_conn, ip, sock_fd, &globals);
			new_conn.detach();
		}
	}
}
