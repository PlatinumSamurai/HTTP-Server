#ifndef NETWORK_H_
#define NETWORK_H_

#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

int Listen(char *address);
int Accept(int listenFd);
int Connect(char *address);
int Send(int fd, char *buffer, size_t size);
int Recv(int fd, char *buffer, size_t size);


#endif  // NETWORK_H_
