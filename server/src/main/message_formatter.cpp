#include "message_formatter.h"

#include <sstream>

#include <iostream>

#include "message_codes.h"

int str_to_int(std::string num)
{
	int value;
	std::stringstream str_int(num);
	str_int >> value;
	return value;
}


/* '|' as dividers
 *
 *
 */
std::vector<std::string> tokenize_msg(std::string msg, int delimiter_ct)
{
	int token_ct = 0;
	int idx, prev_idx = 0;
	std::vector<std::string> tokens;

	// parse for delimiter
	for(idx = 0; idx <= msg.size(); idx++) {
		if(token_ct == delimiter_ct) {
			break;
		}

		if(msg[idx] == CODE_MSG_DIVIDER) {
			tokens.push_back(msg.substr(prev_idx, idx-prev_idx));
			prev_idx = idx+1;
			token_ct++;
		}
	}

	// remaining text is the second token
	if(idx < msg.size()) {
		idx = msg.size();
		tokens.push_back(msg.substr(prev_idx, idx-prev_idx));
	}
	// no delimiter found
	else if(token_ct != delimiter_ct) {
		tokens.push_back(msg.substr(prev_idx, idx-prev_idx));
	}

	return tokens;
}

