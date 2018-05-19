#ifndef MESSAGE_FORMATTER_H_
#define MESSAGE_FORMATTER_H_

#include <string>
#include <vector>

// jump table
// std::string construct_message_header(int msg_len, const char *code);

int str_to_int(std::string num);
std::vector<std::string> tokenize_msg(std::string msg, int delimiter_ct);
std::string construct_msg(std::vector<std::string> tokens);


#endif