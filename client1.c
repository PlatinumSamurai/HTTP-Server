#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


int main() {
    int fd;
    struct sockaddr_in addr;
    char buf[4096];
    char processing[1024];
    char temp[512];
    char result[512];
    FILE *file;
    int connectionCheck;
    char *str1, *str2;
    char *token, *subtoken;
    char *savePtr1, *savePtr2;
    int j;
    char userName[2048] = {0};
    char userBirthDate[2048] = {0};
    char userToken[2048] = {0};


    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("Error occurred while executing socket()");
        return -1;
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = 9000;
    inet_aton("127.0.0.1", &addr.sin_addr);

    for(int i = 0; i < 30; ++i) {
        connectionCheck = connect(fd, (const struct sockaddr *) &addr, sizeof(addr));
        if(connectionCheck == -1) {
            sleep(1);
        } else {
            break;
        }
    }

    if(connectionCheck == -1) {
        perror("Error occurred while executing connect()");
        return -1;
    } else {
        printf("Connected to the server!\n");
    }

    memset(buf, 0, sizeof(buf));

    read(fd, buf, sizeof(buf));

    printf("SERVER'S RESPONSE: %s\n", buf);

    printf("Enter your data\n First name: ");
    scanf("%s", userName);
    write(fd, userName, strlen(userName));

    printf("Birth date in dd.mm.yyyy format: ");
    scanf("%s", userBirthDate);
    write(fd, userBirthDate, strlen(userBirthDate));

    printf("Token: ");
    scanf("%s", userToken);
    write(fd, userToken, strlen(userToken));

    // read(fd, buf, sizeof(buf));

    // printf("SERVER'S RESPONSE: %s\n", buf);


    // sleep(20);
    // sleep(3);


    // for(str1 = buf; ; str1 = NULL) {
    //     token = strtok_r(str1, "\n", &savePtr1);
    //     if(token == NULL) {
    //         break;
    //     }
    //     memset(temp, 0, sizeof(temp));
    //     strcpy(temp, token);
    //     for(j = 1, str2 = token; ; ++j, str2 = NULL) {
    //         subtoken = strtok_r(str2, " ", &savePtr2);
    //         if(subtoken == NULL) {
    //             break;
    //         }
    //         if(j == 3) {
    //             memset(result, 0, sizeof(result));
    //             memset(processing, 0, sizeof(processing));
    //             strcpy(processing, "cut -d: -f1 /etc/passwd | grep -x \"");
    //             strcat(processing, subtoken);
    //             strcat(processing, "\"");
    //             file = popen(processing, "r");
    //             if(fread(result, 1, sizeof(subtoken), file) != 0) {
    //                 write(fd, temp, sizeof(temp));
    //                 // printf("%s\n", temp);
    //             }
    //             pclose(file);
    //         }
    //     }
    // }

    sleep(5);
    shutdown(fd, 2);
    close(fd);

    return 0;
}