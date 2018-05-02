
#include "connection.h"

Connection::Connection(int fd, std::string ip, global_data * data)
{    
    // set class variables
    this->fd = fd;
    this->ip = ip;
    this->data = data;
}

// send a message to intiate neighbor
bool Connection::greet_neighbor()
{

}

// receive and handle messages
bool Connection::receive_message()
{

}

// send a message to neighbor
bool Connection::send_message()
{

}

// place message into global data structure
bool Connection::handle_message()
{

}
