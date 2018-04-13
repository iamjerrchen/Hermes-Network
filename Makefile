
# Makefile for P2P Messaging Network Project


### Flags used for compilation ###
CPPFLAGS=-Wall -fno-builtin -fno-stack-protector
CPPC=g++


### client ###
CLIENT_SRC=./src/temp/client.cpp

client: $(CLIENT_SRC)
	$(CPPC) $(CPPFLAGS) -o $@ $<


### server test ###
SERVER_TEST_SRC=./src/test/server_test.cpp
	# $(wildcard ./src/main/*.cpp)
SERVER_TEST_INC=./src/main/socket.cpp
SERVER_TEST_DEP=./bin/server_test.d

server_test: $(SERVER_TEST_SRC)
	$(CPPC) $(CPPFLAGS) $(SERVER_TEST_INC) -o $@ $<


### cleanup ###
.PHONY: clean
clean:
	rm -rf client server_test

