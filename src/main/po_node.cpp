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
	unsigned int init_idx;
	this->neighbor_ips = new unsigned long[this->max_neighbors];
	this->filled = new bool[this->max_neighbors];
	for(init_idx = 0; init_idx < this->max_neighbors; init_idx++)
	{
		(this->neighbor_ips)[init_idx] = 0;
		(this->filled)[init_idx] = false;
	}
}


PO_Node::~PO_Node()
{
	delete [] this->neighbor_ips;
	delete [] this->filled;
}

// TODO: Change parameter to make it secure from possible overflows
bool PO_Node::add_neighbor(char *ip)
{
	unsigned int filled_idx = 0;
	unsigned long ip_val;
	struct in_addr temp;

	if(inet_aton(ip, &temp) == 0)
	{
		syslog(LOG_NOTICE, "[po_node] Invalid ip address string provided.");
		return false;
	}

	ip_val = temp.s_addr;
	while(filled_idx < this->max_neighbors)
	{
		if( !( (this->filled)[filled_idx] ) )
		{
			(this->filled)[filled_idx] = true;
			(this->neighbor_ips)[filled_idx] = ip_val;
			return true;
		}
	}
	// unable to add more neighbors
	return false;
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
	// iterate over the neighbors
	while(filled_idx < this->max_neighbors)
	{
		if((this->filled)[filled_idx])
		{
			// remove the matching neighbor
			if(ip_check == (this->neighbor_ips)[filled_idx])
			{
				(this->filled)[filled_idx] = false;
				(this->neighbor_ips)[filled_idx] = 0;
				success = true;
			}
		}
	}
	// ip doesn't exist
	return success;
}