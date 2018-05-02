#include "po_node.h"

#include "global_data.h"
#include "connection.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// system logging
#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <iostream>

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *		
 */
void start_neighbor_to_server_conn(std::string ip, int sock_fd, global_data* data)
{
	Connection *stream = new Connection(sock_fd, ip, data);

	stream->receive_message();

	while(0) // change to 1
	{
		// listen for messages and send messages simultaneously
	}


	return;
}

/* 
 * @purpose:
 * 		Main function for a server to neighbor connection thread.
 * @param:
 *
 * @return:	
 *		
 */
 bool start_server_to_neighbor_conn(std::string ip, global_data* data)
 {
 	int sock_fd;
 	Socket client_sock;
 	if((sock_fd = client_sock.setup_client_socket(SERVER_PORT, ip.c_str())) < 0)
 	{	// unable to create client
 		syslog(LOG_NOTICE, "[po_node] Unable to connect to the neighbor ip (%s)", ip);
 		return false;
 	}

 	Connection* stream = new Connection(sock_fd, ip, data);

 	// send initialization message
 	stream->greet_neighbor();

 	while(0) // change to 1
 	{
 		// listen for messages and send messages simultaneously
 	}

 	return true;
}
