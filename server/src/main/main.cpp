#include <stdlib.h>
#include <stdio.h>
#include "global_data.h"

using std;

int main(int argc, char * argv[]) {
	if (argc > 4)
		exit(EXIT_FAILURE);
	int num_connections = argc - 1;

	global_data globals;
	globals.incoming_messages = new map<string,queue>();
	globals.outgoing_messages = new map<string,queue>();

	// Create client connection
	
	// Create node connections
	for (int i = 1; i <= argc; i++) {
		// thread new_conn = new thread(//TODO: connection static method);
		// new_conn.join();
	}
	
	// create server socket
	Socket server_listener = new Socket();
	while (true) {
		int fd = -1;
		// if number of connections < 3, have server accept
		// ensure threads is always <= 3
		if (num_connections < 3) {
			fd = server_listener.accept_conn();
			// thread new_conn = new thread(//TODO: connection static method)
			// new_conn.join();
			num_connections++;
		}
	}
}
