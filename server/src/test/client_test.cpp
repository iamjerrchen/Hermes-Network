// Client side C/C++ program to demonstrate Socket programming

#include "../main/socket.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
 
int main(int argc, char const *argv[])
{
    int sock, valread;
    char *hello = "Hello from client";
    char buffer[1024] = {0};

    Socket s;
    sock = s.setup_client_socket(31337, "127.0.0.1");
    if(sock < 0)
    {
        std::cout << "Failed" << std::endl;
    }

    send(sock , hello , strlen(hello), 0);
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}