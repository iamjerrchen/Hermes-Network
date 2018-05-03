#include "connection.h"
#include "message_codes.h"

#include <sstream>

#include <string.h>
#include <syslog.h>
#include <errno.h>

#include <iostream>

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
	// length including message code, divider, and ip
    int send_len = CODE_LEN + 1 + (this->ip).length();

    // build message
    std::string message = std::to_string(send_len);
    message += CODE_MSG_DIVIDER;
    message += INIT_CODE;
    message += CODE_MSG_DIVIDER;
    message += this->ip;

    // send greetings with message data
    send_len = write(this->fd, message.c_str(), strlen(message.c_str()));
    if(send_len < 0) {
        syslog(LOG_ERR, "[connection] Failed to write to ip (%s): %s", (this->ip).c_str(), strerror(errno));
        return false;
    }
    return true; // if sent otherwise false
}

// receive and handle messages
bool Connection::receive_message()
{
    bool status = false;
    int recv_len, size_idx, expected_len, curr_len;
    std::string message, msg_start;
    char buffer[MAX_BUF_LEN];

    // read a portion of the message for examination
	recv_len = read(this->fd, buffer, MAX_BUF_LEN);
	msg_start.append(buffer, recv_len);
	if((size_idx = seek_divider(msg_start)) < 0) {
		syslog(LOG_WARNING, "[connection] Unable to find the end of message length metadata.");
		return false;
	}

	// convert from string to int
	std::string message_length = msg_start.substr(0,size_idx);
	std::stringstream str_int(message_length);
	str_int >> expected_len;

    // read to end of message
	curr_len = recv_len-(size_idx+1);
	message = msg_start.substr(size_idx+1, curr_len);
    while(curr_len < expected_len) {
    	recv_len = read(this->fd, buffer, MAX_BUF_LEN);
        if(recv_len > 0) {
            message.append(buffer, recv_len);
            status = true;
        }
        else if(recv_len < 0) {
            syslog(LOG_ERR, "[connection] Failed to read from ip (%s): %s", (this->ip).c_str(), strerror(errno));
            return false;
        }
    	curr_len += recv_len;
    }

    // TODO: Store message
    std::cout << message << std::endl;
    return status;
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


/*
 * @purpose:
 *      
 *
 * @param:
 *      message: string starting with 4 character message code
 * 			followed by a variable message length count and another
 *			divider.
 *			ex. XX...XX|XXXX|MSG
 * @return: index to the end of the message length
 *      > 0: Valid index to the last message length number
 *      -1: Unable to find end of message length
 */
int Connection::seek_divider(std::string message)
{
    bool found = false;
    unsigned int idx;
    for(idx = 0; idx < message.length(); idx++) {
        if(message[idx] == CODE_MSG_DIVIDER) {
            found = true;
            break;
        }
    }

    if(found) {
        return int(idx);
    }

    return -1;
}