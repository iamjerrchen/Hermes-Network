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
    char* message = "Hello";
    int n = write(this->fd, message, strlen(message));
    //TODO: log n<0 (error)
}

// receive and handle messages
bool Connection::receive_message()
{
    char buffer[256];
    int n = read(this->fd, buffer, 255);
    // TODO: error report if n<0
}

// send a message to neighbor
bool Connection::send_message()
{

}

// place message into global data structure
bool Connection::handle_message()
{

}
