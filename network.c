#include "network.h"

typedef enum error_t {
    SOCKET_ERR,
    BIND_ERR,
    LISTEN_ERR,
    CONNECT_ERR,
    ACCEPT_ERR,
    SEND_ERR,
    RECV_ERR
} error_t;

int Listen(char *address) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    char ip[16] = "127.0.0.1";
    char port[6] = "11111";
    struct sockaddr_in addr;


    if(fd == -1) {
        perror("1");
        return LISTEN_ERR;
    }

/*	if (_parse_address(address, ipv4, port) != 0) {
		return PARSE_ERR;
	}
*/
    addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
		return BIND_ERR;
	}
	if (listen(fd, SOMAXCONN) != 0) {
		return LISTEN_ERR;
	}
	return fd;
}

int Accept(int listenFd) {
    int fd;
    time_t start = clock();

    do {
        //printf("TIMER = %ld\n", (clock() - start) / CLOCKS_PER_SEC);
        if((clock() - start) / CLOCKS_PER_SEC > 10) {
            break;
        }   
        fcntl(listenFd, F_SETFL, O_NONBLOCK);
        fd = accept(listenFd, NULL, NULL);
        //printf("Client's fd = %d\n", fd);
    } while(fd == -1 && errno == EAGAIN);

    //printf("Client's fd = %d\n", fd);
    if(fd == -1) {
        perror("2");
    }
    
    return fd;
}

int Connect(char *address) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
	char ip[16] = "127.0.0.1";
    char port[6] = "11111";
    struct sockaddr_in addr;
    
    if (fd < 0) {
		return SOCKET_ERR;
	}
/*	
	if (_parse_address(address, ipv4, port) != 0) {
		return PARSE_ERR;
	}
*/

	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
//	addr.sin_addr.s_addr = inet_addr(ip);
    inet_aton(ip, &addr.sin_addr);
	
    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("3");
        return CONNECT_ERR;
	}

	return fd;
}

int Send(int fd, char *buffer, size_t size) {
    printf("====================================================================================================================================\ninside function Send. Buffer to send is \n %s \n\n", buffer);
	int bytes = send(fd, buffer, (int)size, 0);

    if(bytes == -1) {
        perror("4");
        return SEND_ERR;
    }

    return bytes;
}

int Recv(int fd, char *buffer, size_t size) {
	int bytes = recv(fd, buffer, (int)size, 0);

    if(bytes == -1) {
        perror("5");
        return RECV_ERR;
    }
// TODO what if connection is closed and zero bytes are read ?
    return bytes;
}

int Close(int fd) {
    close(fd);
    return 0;
}
