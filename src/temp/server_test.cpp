#include "socket.h"

#include <stdio.h>
#include <unistd.h> // read()
#include <string.h>

int main()
{
	int new_socket, valread;

	char buffer[1024] = {0};
	char *hello = "Hello from server";

	Socket s;
	s.create();
	s.attach();
	s.listen_conn();
	new_socket = s.accept_conn();

	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	send(new_socket, hello, strlen(hello), 0);
	std::cout << "Hello message sent\n" std::endl;
	return 0;
}


