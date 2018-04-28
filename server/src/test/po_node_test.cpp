/* po_node_test.cpp
 *
 *
 *
 */

#include "../main/po_node.h"

#include <iostream>
#include <string>

// will need to make class variables public or getter classes
// void print_filled(PO_Node server)
// {
// 	std::cout << "Filled neighbors:" << std::endl;
// 	for(unsigned int idx = 0; idx < server.max_neighbors; idx++)
// 	{
// 		std::cout << idx << " : " << server.filled[idx] << ", " << server.neighbor_ips[idx] << std::endl;
// 	}
// }

int main()
{
	// test add_neighbor to make sure neighbor_node_ips are filled properly.
	std::string ip1 = "127.0.0.1";
	std::string ip2 = "127.0.0.2";
	std::string ip3 = "127.0.0.3";
	std::string invalid_ip = "AAAAAA";

	PO_Node server;
	// print_filled(server);
	// attempt to add the same ip multiple times
	for(int x = 0; x < 4; x++)
	{
		if(server.add_neighbor_ip((char*)ip1.data()))
		{
			std::cout << "Successfully added ip: " + ip1 << std::endl;
		}
		else
		{
			std::cout << "Unable to add ip: " + ip1 << std::endl;
		}
	}
	// print_filled(server);

	// add other two ips
	if(server.add_neighbor_ip((char*)ip2.data()))
	{
		std::cout << "Successfully added ip: " + ip2 << std::endl;
	}
	else
	{
		std::cout << "Unable to add ip: " + ip2 << std::endl;
	}
	if(server.add_neighbor_ip((char*)ip3.data()))
	{
		std::cout << "Successfully added ip: " + ip3 << std::endl;
	}
	else
	{
		std::cout << "Unable to add ip: " + ip3 << std::endl;
	}

	// remove second ip
	if(server.remove_neighbor_ip((char*)ip2.data())) {
		std::cout << "Successfully removed ip: " + ip2 << std::endl;
	}
	// print_filled(server);

	return 0;
}
