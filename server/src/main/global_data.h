#ifndef GLOBAL_DATA_H_
#define GLOBAL_DATA_H_

#include <queue>
#include <map>
#include <string>
#include <thread>
#include <mutex>

struct global_data {
	std::map<std::string, std::queue<std::string>> *incoming_messages;
	std::map<std::string, std::queue<std::string>> *outgoing_messages;
	std::mutex in_lock;
	std::mutex out_lock;
};

#endif
