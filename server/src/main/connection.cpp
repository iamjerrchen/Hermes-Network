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
    return true; // if sent otherwise false
}

// receive and handle messages
bool Connection::receive_message()
{
    char buffer[256];
    int n;
    if((n = read(this->fd, buffer, 255)) > 0)
    {
        // TODO: Store message
        printf("Message received: %s\n", buffer);
        // TODO: error report if n<0
        return true;
    }

    return false;
}

// send a message to neighbor
bool Connection::send_message()
{
    char* buffer = "Got your message";
    int n = write(this->fd, buffer, strlen(buffer));
    return true;
}

// place message into global data structure
bool Connection::handle_message()
{
    return false;
}
