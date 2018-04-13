#include "../main/socket.h"

#include <stdio.h>
#include <unistd.h> // read()
#include <string.h>

int main()
{
	int new_socket, valread;

	char buffer[1024] = {0};
	char *hello = "Hello from server";

	Socket s;
	s.setup_server_sock(31337);

	// Continually listen on the port and accept connections.
	while(1)
	{
		new_socket = s.accept_conn();
		valread = read(new_socket, buffer, 1024);
		printf("%s\n", buffer);
		send(new_socket, hello, strlen(hello), 0);
		printf("Hello message sent\n");
	}
	return 0;
}


