#include "../network.h"

int main() {
    char buf[100];

    int fd = Connect("127.0.0.1");

    Recv(fd, buf, 100);
    
    Send(fd, "Bye", 4);
    printf("server's response: %s\n", buf);

    sleep(3);

    close(fd);

    return 0;
}
