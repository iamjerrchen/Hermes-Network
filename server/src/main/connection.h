#ifndef P2P_MAIN_CONNECTION_H_
#define P2P_MAIN_CONNECTION_H_

#include <string.h>

#include "socket.h"
#include "global_data.h"
class Connection {

private:

    int fd; // file descriptor
    std::string ip; // ip address of other side of connection
    global_data * data; // pointer to global map of messages

public:

    // constructor and destructor
    Connection(int fd, std::string ip, global_data * data);
    ~Connection();
    

    // methods to interface with messages
    bool greet_neighbor();
    bool receive_message();
    bool send_message();
    bool handle_message();

};

#endif
