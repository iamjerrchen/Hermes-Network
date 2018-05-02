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

// private functions
static bool spawn_client_listener();
static bool add_neighbor_ip(unsigned long ip_val);
// TODO: remove_neighbor_ip() by internal idx
static bool remove_neighbor_ip(unsigned long ip_check);

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
//FIXME
bool spawn_client_listener()
{
	client_connection = new Socket();
	if(!client_connection->setup_server_socket(CLIENT_PORT))
	{
		syslog(LOG_ALERT, "[po_node] Server is unable to create client listener for port: %u.", CLIENT_PORT);
		return false;
	}

	syslog(LOG_NOTICE, "[po_node] Server has spawned client listener. Ready to accept connections.");
	return true;
}



/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *		
 */
void start_neighbor_to_server_conn(std::string ip, int sock_fd)
{
	Connection* stream = new Connection(sock_fd, ip.c_str(), &global_data);

	stream.receive_message();

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
 bool start_server_to_neighbor_conn(std::string ip)
 {
 	int sock_fd;
 	Socket client_sock;

 	if((sock_fd = client_sock.setup_client_socket(ip.c_str(), SERVER_PORT)) < 0)
 	{	// unable to create client
 		syslog(LOG_NOTICE, "[po_node] Unable to connect to the neighbor ip (%s)", ip);
 		return false;
 	}

 	Connection* stream = new Connection(sock_fd, ip.c_str(), &global_data);

 	// send initialization message
 	stream.greet_neighbor();

 	while(0) // change to 1
 	{
 		// listen for messages and send messages simultaneously
 	}

 	return true;
}

/* ----------------------------------------------------------------- */

/* 
 * @purpose:
 * 		Adds the provided ip in string form to neighbor_ips
 * @param:
 *		ip | ip in the form of a string, ex. "127.0.0.1"
 * @return:	
 *		true: properly executed operation
 *		false: failed to add neighbor
 */
// TODO: Change parameter to make it secure from possible overflows
bool add_neighbor_ip(char *ip)
{
	unsigned long ip_val;
	struct in_addr temp;

	if(inet_aton(ip, &temp) == 0)
	{
		syslog(LOG_NOTICE, "[po_node] Invalid ip address string provided.");
		return false;
	}

	ip_val = temp.s_addr;
	return add_neighbor_ip(ip_val);
}

/* private
 * @purpose:
 * 		Adds provided ip in byte form to class neighbor_ips
 * @param:
 *		ip_val | provided ip in byte form
 * @return:	status of operation
 *		true: properly executed operation
 *		false: failed to add neighbor ip
 */
// internal: add neighbor to list provided binary ip
static bool add_neighbor_ip(unsigned long ip_val)
{
	bool success = false;
	bool ip_exists = false;
	
	unsigned int filled_idx = 0;
	unsigned int avail_idx;

	// find last available slot and if ip already exists
	while(filled_idx < MAX_NEIGHBORS)
	{	
		if( !(filled[filled_idx]) ) // empty slot
		{
			avail_idx = filled_idx;
		}
		else if(neighbor_ips[filled_idx] == ip_val) // ip exists
		{			
			ip_exists = true;
		}
		filled_idx++;
	}

	// adding new neighbor
	if(!ip_exists)
	{
		filled[avail_idx] = true;
		neighbor_ips[avail_idx] = ip_val;
		success = true;
		// spawn_server_listener(avail_idx);
	}

	// unable to add more neighbors
	if(ip_exists)
	{
		// TODO: Does syslog have a stack smash vulnerability
		syslog(LOG_NOTICE, "[po_node] Attempted to add an existing neighbor ip.");
	}
	else if(!success)
	{
		syslog(LOG_NOTICE, "[po_node] Max %d neighbors reached. Unable to add more.", MAX_NEIGHBORS);
	}
	return success;
}

/* public
 * @purpose:
 * 		Removes specified ip in string form from class neighbor_ips
 * @param:
 *		ip | ip in string form
 * @return:	status of operation
 *		true: successfully removed neighbor
 *		false: invalid ip provided or nonexistent ip
 */
// TODO: Change parameter to make it secure from possible overflows
bool PO_Node::remove_neighbor_ip(char *ip)
{
	unsigned long ip_check;
	struct in_addr temp;

	// TODO: Double check logic, consider printing the provided string
	if(inet_aton(ip, &temp) == 0)
	{
		syslog(LOG_NOTICE, "[po_node] Invalid ip address string provided.");
		return false;
	}

	ip_check = temp.s_addr;
	return remove_neighbor_ip(ip_check);
}

/* private
 * @purpose:
 * 		Removes specified ip in byte order form from class neighbor_ips
 * @param:
 *		ip_check | ip in byte order form
 * @return:	status of operation
 *		true: successfully executed operation
 *		false: failed to remove nonexistent ip
 */
static bool PO_Node::remove_neighbor_ip(unsigned long ip_check)
{
	bool success = false;
	
	unsigned int filled_idx = 0;

	// iterate over the neighbors, and remove the first matching ip
	while(filled_idx < MAX_NEIGHBORS)
	{
		if(filled[filled_idx])
		{
			// remove the matching neighbor
			if(ip_check == neighbor_ips[filled_idx])
			{
				filled[filled_idx] = false;
				neighbor_ips[filled_idx] = 0;
				success = true;
				break;
			}
		}
		filled_idx++;
	}
	
	// ip doesn't exist
	if(!success)
	{
		// TODO: Does syslog have a stack smash vulnerability
		syslog(LOG_NOTICE, "[po_node] Can't delete nonexistent neighbor ip.");
	}
	return success;
}
