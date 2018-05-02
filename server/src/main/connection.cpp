
#include "connection.h"

Connection::Connection(int fd, std::string ip, global_data * data)
{    
    // set class variables
    this->fd = fd;
    this->ip = ip;
    this->data = data;
}

Connection::Connection(int fd, global_data * data)
{
    // create an fd from the socket object
    this->fd = fd;
    this->ip = "";
    this->data = data;
}

// send a message to intiate neighbor
bool Connection::greet_neighbor()
{

}

bool Connection::receive_message()
{

}

bool Connection::send_message()
{

}

bool Connection::handle_message()
{

}
