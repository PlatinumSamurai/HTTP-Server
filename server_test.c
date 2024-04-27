#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>


struct user {
    int sock;
    int infSent;
};


char* Hash(char *buf) {
    return "hashed value\n";
}

struct user* acceptClient(int fd, struct user *clients, int *clientsNum) {
    int sock = accept(fd, NULL, NULL);
    struct user *users;

    printf("Accepted new client! %d\n", sock);
    if(sock < 0) {
        perror("Error occurred while executing accept()");
        return clients;
    }
    fcntl(sock, F_SETFL, O_NONBLOCK);

    if(*clientsNum == 0) {
        users = (struct user *)malloc(sizeof(struct user));
    } else {
        users = (struct user *)realloc(clients, (*clientsNum + 1) * sizeof(struct user));
    }
    if (users == NULL) {
        users = clients;
    } else {
        (*clientsNum)++;
        users[(*clientsNum) - 1].sock = sock;
        users[(*clientsNum) - 1].infSent = 0;
    }

    return users;
}


struct user* disconectUser(int userNum, struct user *clients, int *clientsNum) {
    struct user *new;

    if(*clientsNum - 1 == 0) {
        (*clientsNum)--;
        free(clients);
        return NULL;
    }

    new = malloc(sizeof(struct user) * (*clientsNum - 1));

    for(int i = 0, j = 0; j < *clientsNum; ++j) {
        if(j != userNum) {
            new[i++] = clients[j];
        }
    }

    free(clients);
    (*clientsNum)--;

    return new;
}


void sentInfo(struct user *clients, int clientsNum, fd_set *writeArr, char *buf2) {
    for(int i = 0; i < clientsNum; ++i) {
        if(clients[i].sock != -1 && clients[i].infSent == 0) {
            if(FD_ISSET(clients[i].sock, writeArr)) {
                send(clients[i].sock, buf2, sizeof(buf2), 0);
                clients[i].infSent = 1;
            }
        }
    }
}


int main() {
    int fd;
    struct sockaddr_in addr;
    struct user *clients = NULL;
    int clientsNum = 0;
    char buf[2048];
    char buf2[2048] = "HELLOOOOOOOOO!!!!!!!\n\0";


    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("Error occurred while executing socket()");
        return -1;
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = 9000;
    inet_aton("127.0.0.1", &addr.sin_addr);

    fcntl(fd, F_SETFL, O_NONBLOCK);

    if(bind(fd, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
        perror("Error occurred while executing bind()");
        shutdown(fd, 2);
        close(fd);
        return -1;
    }

    listen(fd, 1);

    while(1) {
        fd_set readArr;
        fd_set writeArr;
        FD_ZERO(&readArr);
        FD_ZERO(&writeArr);
        FD_SET(fd, &readArr);
        int max = fd;

        for(int i = 0; i < clientsNum; ++i) {
            FD_SET(clients[i].sock, &readArr);
            FD_SET(clients[i].sock, &writeArr);
            if(clients[i].sock > max) {
                max = clients[i].sock;
            }
        }
        
        struct timeval timeout;
        timeout.tv_sec = 7;
        timeout.tv_usec = 0;

        if(select(max + 1, &readArr, &writeArr, NULL, &timeout) <= 0) {
            printf("Nobody wants to communicate with me :(\n");
            goto end;
        }

        if(FD_ISSET(fd, &readArr)) {
            clients = acceptClient(fd, clients, &clientsNum);
        }


        // SENT INFO TO CLIENT
        // sentInfo(clients, clientsNum, &writeArr, buf2);
        for(int i = 0; i < clientsNum; ++i) {
            if(/*clients[i].sock != -1 &&*/ clients[i].infSent == 0) {
                if(FD_ISSET(clients[i].sock, &writeArr)) {
                    send(clients[i].sock, buf2, sizeof(buf2), 0);
                    clients[i].infSent = 1;
                }
            }
        }

        for(int i = 0; i < clientsNum; ++i) {
            if(clients[i].sock != -1 && FD_ISSET(clients[i].sock, &readArr)) {
                int bytesRead = recv(clients[i].sock, buf, sizeof(buf), 0);
                // printf("Bytes read: %d\n", bytesRead);
                if(bytesRead > 0) {
                    printf("\t\tCLIENT'S %d RESPONSE:\n", clients[i].sock);
                    printf("%s\n", buf);
                    memset(buf, 0, strlen(buf));
                    
                    char *hash = Hash(buf);
                    /*
                    if(hash == user's val) {
                        if(FD_ISSET(clients[i].sock, &writeArr)) {
                            char str[256] = "success! My innermost secret is that I want some pizza...\n\0";
                            send(clients[i].sock, str, sizeof(str), 0);
                            clients[i].infSent = 2;
                        }
                    }
                    */
                }

                if(bytesRead <= 0) {
                    printf("CLIENT %d DISCONNECTED...\n", clients[i].sock);
                    close(clients[i].sock);
                    clients = disconectUser(i, clients, &clientsNum);
                }
            }
        }

        // USER'S RESPONSE
        // for(int i = 0; i < clientsNum; ++i) {
        //     if(clients[i].sock != -1 && FD_ISSET(clients[i].sock, &readArr)) {
        //         int bytesRead = recv(clients[i].sock, buf, sizeof(buf), 0);
        //         // printf("Bytes read: %d\n", bytesRead);
        //         if(bytesRead > 0) {
        //             printf("\t\tCLIENT'S %d RESPONSE:\n", clients[i].sock);
        //             printf("%s\n", buf);
        //         }

        //         if(bytesRead <= 0) {
        //             close(clients[i].sock);
        //             printf("CLIENT %d DISCONNECTED...\n", clients[i].sock);
        //             clients[i].sock = -1;
        //             clients[i].infSent = 0;
        //             continue;
        //         }
        //     }
        // }
    }

end:
    shutdown(fd, 2);
    close(fd);
    free(clients);

    return 0;
}