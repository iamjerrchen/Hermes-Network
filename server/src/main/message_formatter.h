#ifndef MESSAGE_FORMATTER_H_
#define MESSAGE_FORMATTER_H_

#include <string>
#include <vector>

// jump table
// std::string construct_message_header(int msg_len, const char *code);


std::vector<std::string> extract_message(std::string msg, int delimiter_ct);


#endif