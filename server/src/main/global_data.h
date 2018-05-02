#ifndef GLOBAL_DATA_H_
#define GLOBAL_DATA_H_
#include <queue>
#include <map>
#include <string>
#include <thread>
#include <mutex>

struct global_data {
	map<string,queue> *incoming_messages;
	map<string,queue> *outgoing_messages;
	mutex in_lock;
	mutex out_lock;
};
#endif
