#include "po_node.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// system logging
#include <string.h>
#include <syslog.h>
#include <errno.h>

#include <iostream>

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

// destructor
PO_Node::~PO_Node()
{
	delete [] this->neighbor_ips;
	delete [] this->filled;
}

// copy constructor
PO_Node::PO_Node(const PO_Node &other)
{
	if(this != &other)
	{	// this->max_neighbors is const so they should have the same initialization
		this->neighbor_ips = new unsigned long[this->max_neighbors];
		this->filled = new bool[this->max_neighbors];
		for(unsigned int cpy_idx = 0; cpy_idx < this->max_neighbors; cpy_idx++)
		{
			(this->neighbor_ips)[cpy_idx] = (other.neighbor_ips)[cpy_idx];
			(this->filled)[cpy_idx] = (other.filled)[cpy_idx];
		}
	}
}

// assignment overload
PO_Node & PO_Node::operator=(const PO_Node &rhs)
{
	if(this != &rhs)
	{	// this->max_neighbors is const so they should have the same initialization
		this->neighbor_ips = new unsigned long[this->max_neighbors];
		this->filled = new bool[this->max_neighbors];
		for(unsigned int cpy_idx = 0; cpy_idx < this->max_neighbors; cpy_idx++)
		{
			(this->neighbor_ips)[cpy_idx] = (rhs.neighbor_ips)[cpy_idx];
			(this->filled)[cpy_idx] = (rhs.filled)[cpy_idx];
		}
	}

	return *this;
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
	while(filled_idx < this->max_neighbors)
	{	
		if( !((this->filled)[filled_idx]) ) // empty slot
		{
			avail_idx = filled_idx;
		}
		else if((this->neighbor_ips)[filled_idx] == ip_val) // ip exists
		{			
			ip_exists = true;
		}
		filled_idx++;
	}

	if(!ip_exists)
	{
		(this->filled)[avail_idx] = true;
		(this->neighbor_ips)[avail_idx] = ip_val;
		success = true;
	}

	// unable to add more neighbors
	if(ip_exists)
	{
		// TODO: Does syslog have a stack smash vulnerability
		syslog(LOG_NOTICE, "[po_node] Attempted to add an existing neighbor ip (%s).", ip);
	}
	else if(!success)
	{
		syslog(LOG_NOTICE, "[po_node] Max %d neighbors reached. Unable to add more.", this->max_neighbors);
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