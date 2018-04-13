
# Makefile for P2P Messaging Network Project
# All executables will be stored in ./bin/proj/
# ***Unit Test Makefile located in test directory

### Flags used for compilation ###
CPPC=g++
CPPFLAGS=-Wall -fno-builtin -fno-stack-protector
BIN_DIR=./bin/proj/$@ $<


### server test ###
# MAIN_SRC= # project main file
	# $(wildcard ./src/main/*.cpp)
# MAIN_INC= # Local files included in the project main file

# P2P: $(MAIN_SRC)
	# $(CPPC) $(CPPFLAGS) $(MAIN_INC) -o $(BIN_DIR)


### all ###
# all:


### cleanup ###
.PHONY: clean
clean:
	rm -rf ./bin/proj/*

