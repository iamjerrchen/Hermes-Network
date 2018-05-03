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
	this->local_incoming_msg = new std::queue<std::string>();
	this->local_outgoing_msg = new std::queue<std::string>();

	// Initialize global maps for this IP
	{
		std::lock_guard<std::mutex> lock(data->in_lock);
		if (data->incoming_messages->count(ip) == 0) {
			data->incoming_messages->insert(std::pair<std::string, std::queue<std::string>*>(ip, new std::queue<std::string>()));
		}
	}
	{
		std::lock_guard<std::mutex> lock(data->out_lock);
		if (data->outgoing_messages->count(ip) == 0) {
			data->outgoing_messages->insert(std::pair<std::string, std::queue<std::string>*>(ip, new std::queue<std::string>()));
		}
	}
}

Connection::~Connection() {
	// Remove local objects
	delete local_incoming_msg;
	delete local_outgoing_msg;

	// Remove global objects
	{
		std::lock_guard<std::mutex> lock(data->in_lock);
		delete data->incoming_messages->at(ip);
	}
	{
		std::lock_guard<std::mutex> lock(data->out_lock);
		delete data->outgoing_messages->at(ip);
	}
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
    send_len = write(this->fd, message.c_str(), message.length());
    if(send_len < 0) {
        syslog(LOG_ERR, "[connection] Failed to write to ip (%s): %s", (this->ip).c_str(), strerror(errno));
        return false;
    }
    return true; // if sent otherwise false
}

// receive and locally save messages
void Connection::receive_message()
{
    int recv_len, size_idx, expected_len, curr_len;
    std::string message, msg_start;
    char buffer[MAX_BUF_LEN];

	while (1) {
		// read a portion of the message for examination
		recv_len = read(this->fd, buffer, MAX_BUF_LEN);
		if (recv_len == 0) {
			continue;
		}

		msg_start.append(buffer, recv_len);
		if((size_idx = seek_divider(msg_start)) < 0) {
			syslog(LOG_WARNING, "[connection] Unable to find the end of message length metadata.");
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
			}
			else if(recv_len < 0) {
				syslog(LOG_ERR, "[connection] Failed to read from ip (%s): %s", (this->ip).c_str(), strerror(errno));
				return;
			}
			curr_len += recv_len;
		}

		// Store message into local stack
		{
			std::lock_guard<std::mutex> lock(local_in);
			local_incoming_msg->push(message);
		}
	}
}

// send a message to neighbor
void Connection::send_message()
{
	int content_len, send_len;
	std::string msg;

	while (1) {
		{
			std::lock_guard<std::mutex> lock(local_out);
			if (local_outgoing_msg->empty()) {
				continue;
			}

			msg = local_outgoing_msg->front();
			local_outgoing_msg->pop();
		}

		// assuming popped message is in MSG_CODE|MESSAGE format
		content_len = msg.length();
		msg =  std::to_string(content_len) + msg;

		// write message
		send_len = write(this->fd, msg.c_str(), msg_length());
		if(send_len < 0)
		{
			syslog(LOG_ERR, "[connection] Failed to write message to ip (%s): %s", (this->ip).c_str(), strerror(errno));
			return;
		}
	}
}

// Worker thread that updates the 2 queues
void Connection::handle_message()
{
	while(1) {
		// Check to update structures every 5 seconds
		sleep(5);

		// Push all local messages to the global map
		{
			std::lock_guard<std::mutex> lock(local_in);
			if (!local_incoming_msg->empty()) {
				std::lock_guard<std::mutex> lock(data->in_lock); // Should not always need to be grabbed
				while (!local_incoming_msg->empty()) {
					data->incoming_messages->at(ip)->push(local_incoming_msg->front());
					local_incoming_msg->pop();
				}
			}
		}

		// Pull pending messages from the global map
		{
			std::lock_guard<std::mutex> lock_external(data->out_lock);
			std::lock_guard<std::mutex> lock_internal(local_out); // Grab this second as it should be faster to get
			while (!data->outgoing_messages->at(ip)->empty()) {
				local_outgoing_msg->push(data->outgoing_messages->at(ip)->front());
				data->outgoing_messages->at(ip)->pop();
			}
		}
	}
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