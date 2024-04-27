#include "../network.h"

int main() {
    char buf[100];

    int fd = Listen("127.0.0.1");
    
    int clientFd = Accept(fd);

    Send(clientFd, "Hello", 6);

    Recv(clientFd, buf, 100);
    printf("client's response: %s\n", buf);

    sleep(5);
    close(fd);

    return 0;
}
