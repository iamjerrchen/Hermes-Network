#include "po_node.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// system logging
#include <string.h>
#include <syslog.h>
#include <errno.h>

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
PO_Node::PO_Node()
{
	client_connection = NULL;
	neighbor_server_sock.setup_server_socket(SERVER_PORT);

	unsigned int init_idx;
	for(init_idx = 0; init_idx < MAX_NEIGHBORS; init_idx++)
	{
		neighbor_ips[init_idx] = 0;
		filled[init_idx] = false;
		// neighbor_threads[init_idx] = NULL;
	}

	// request for neighbor nodes from super node.

	// call add neighbor for each neighbor ip returned
}

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
// destructor
PO_Node::~PO_Node()
{
	// join all threads and delete
	/*for(unsigned int del_idx = 0; del_idx < MAX_NEIGHBORS; del_idx++)
	{	// if thread exists
		if(filled[del_idx])
		{	// if for some reason thread doesn't have an object
			if(NULL != neighbor_threads[del_idx])
			{	// TODO: Consider detach or ensure that threads dont stall
				(neighbor_threads[del_idx])->join();
				delete neighbor_threads[del_idx];
			}
		}
	}*/

	delete client_connection;
}

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
// copy constructor
PO_Node::PO_Node(const PO_Node &other)
{
	if(this != &other)
	{	// this->max_neighbors is const so they should have the same initialization
		for(unsigned int cpy_idx = 0; cpy_idx < MAX_NEIGHBORS; cpy_idx++)
		{
			neighbor_ips[cpy_idx] = (other.neighbor_ips)[cpy_idx];
			filled[cpy_idx] = (other.filled)[cpy_idx];
		}
	}
}

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
// assignment overload
PO_Node & PO_Node::operator=(const PO_Node &rhs)
{
	if(this != &rhs)
	{	// this->max_neighbors is const so they should have the same initialization
		for(unsigned int cpy_idx = 0; cpy_idx < MAX_NEIGHBORS; cpy_idx++)
		{
			neighbor_ips[cpy_idx] = (rhs.neighbor_ips)[cpy_idx];
			filled[cpy_idx] = (rhs.filled)[cpy_idx];
		}
	}

	return *this;
}

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
void PO_Node::server_listen()
{

}

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
// spawns a thread to listen to a port.
bool PO_Node::spawn_server_listener(unsigned int thread_idx)
{
	// initialize pointer with an object. Provide port, 
	// neighbor_threads[thread_idx] = new std::thread(server_listen);
	// provide it a port
	return false;
}

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
bool PO_Node::spawn_client_listener()
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
// TODO: Change parameter to make it secure from possible overflows
// remove neighbor provided string ip
bool PO_Node::add_neighbor_ip(char *ip)
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

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
/* TODO: Neighbor has contacted us to be their neighbor
 * 		 This should be the start of a new thread.
 *
 *
 *
 */
bool PO_Node::add_neighbor_to_server()
{
	return false;
}

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
// TODO: Test this method
// Add and contact the new neighbor.
bool PO_Node::add_server_to_neighbor(unsigned long neighbor_ip)
{
	// keep track of this thread's idx
	int sock_fd; // temporary, move into class variable
	// super node has provided a neighbor to connect to
	// add the ip locally
	add_neighbor_ip(neighbor_ip); // have this return idx
	// create client socket
	// sock_fd = (this->server_neighbor_sock).setup_client_socket();

	// send init message to connect with neighbor server


	// spawn thread to handle connection
	return false;
}

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
// internal: add neighbor to list provided binary ip
bool PO_Node::add_neighbor_ip(unsigned long ip_val)
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

/* 
 * @purpose:
 * 
 * @param:
 *
 * @return:	
 *
 */
// TODO: Change parameter to make it secure from possible overflows
// remove neighbor provided string ip
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
 * 
 * @param:
 *
 * @return:	status of operation
 *
 */
// internal: remove neighbor from list provided binary ip
bool PO_Node::remove_neighbor_ip(unsigned long ip_check)
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