#include "po_node.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// system logging
#include <string.h>
#include <syslog.h>
#include <errno.h>

PO_Node::PO_Node()
{
	client_connection = NULL;
	neighbor_listener.setup_server_socket(SERVER_PORT);

	unsigned int init_idx;
	for(init_idx = 0; init_idx < MAX_NEIGHBORS; init_idx++)
	{
		neighbor_ips[init_idx] = 0;
		filled[init_idx] = false;
		// neighbor_threads[init_idx] = NULL;
	}
}

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

void PO_Node::server_listen()
{

}

// spawns a thread to listen to a port.
bool PO_Node::spawn_server_listener(unsigned int thread_idx)
{
	// initialize pointer with an object. Provide port, 
	// neighbor_threads[thread_idx] = new std::thread(server_listen);
	// provide it a port
	return false;
}

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


// TODO: Change parameter to make it secure from possible overflows
bool PO_Node::add_neighbor(char *ip)
{
	bool success = false;
	bool ip_exists = false;
	
	unsigned int filled_idx = 0;
	unsigned int avail_idx;

	unsigned long ip_val;
	struct in_addr temp;

	if(inet_aton(ip, &temp) == 0)
	{
		syslog(LOG_NOTICE, "[po_node] Invalid ip address string provided.");
		return success;
	}

	ip_val = temp.s_addr;
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
		syslog(LOG_NOTICE, "[po_node] Attempted to add an existing neighbor ip (%s).", ip);
	}
	else if(!success)
	{
		syslog(LOG_NOTICE, "[po_node] Max %d neighbors reached. Unable to add more.", MAX_NEIGHBORS);
	}
	return success;
}

// TODO: Change parameter to make it secure from possible overflows
bool PO_Node::remove_neighbor(char *ip)
{
	bool success = false;
	
	unsigned int filled_idx = 0;

	unsigned long ip_check;
	struct in_addr temp;

	// TODO: Double check logic, consider printing the provided string
	if(inet_aton(ip, &temp) == 0)
	{
		syslog(LOG_NOTICE, "[po_node] Invalid ip address string provided.");
		return success;
	}

	ip_check = temp.s_addr;
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
		syslog(LOG_NOTICE, "[po_node] Can't delete nonexistent neighbor ip (%s).", ip);
	}
	return success;
}