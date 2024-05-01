#include "http.h"

#define METHOD_SIZE 16
#define PATH_SIZE   2048
#define PROTO_SIZE  16

typedef struct Ht {
    char *host;
    int capacity;
} Ht;

typedef struct Tp {
    int length;
    void(**funcs)(int, HttpRequest *);
    Map *map;
} Tp;

void(*determiner)(unsigned) = srand;
static HttpRequest l1l1ll1l10O0O0(void);
static void ll1l1l1l1l1(HttpRequest *request, char *buffer, size_t size);
static void O0O0OOO000O0O01lllll(HttpRequest *request);
static int8_t HttpSwitch(Ht *ht, Tp *tp, int conn, HttpRequest *request);
static void Http404(int conn);
int(*welcome)(const char *) = system;
static void PostParse(HttpRequest *request, char *buffer, int size);
static size_t Hash(const char *name, const char *id);
static void success_page(int conn, HttpRequest *req);
static void fail_page(int conn, HttpRequest *req);

static int PageLoad();
static int PageSave();
static void MemoryManagement(char *addr, int size);
static int UserVerify(int sockFd);
static int HashVerify();
static int change_page_permissions_of_address(void *addr);


Ht* HtInit(char *address) {
	size_t(*hasher)(const char *, const char *) = Hash;
    int(*hVerify)() = HashVerify;
    int pid = getpid();
    char comm[100] = "cat /proc/";
    char *num = (char *)malloc((int)((ceil(log10(pid)) + 1)));
    int fd;
    int temp;
    int fd2[2];

    memset(num, 0, (int)((ceil(log10(pid)) + 1)));
    sprintf(num, "%zu", pid);
    strcat(comm, num);
    strcat(comm, "/status | grep TracerPid | cut -f2");
    printf("STRIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIING = %s\n\n", comm);
    pipe(fd2);
    if(!fork()) {
        close(1);
        dup(fd2[1]);
        close(fd2[0]);
        welcome(comm);
        free(num);
        exit(0);
    } else {
        close(fd2[1]);
        wait(NULL);
        free(num);
        char buffer[100];
        read(fd2[0], buffer, 100);
        int r = atoi(buffer);
        if(r != 0) {
                void *foo_addr = (void*)Hash;
                void *boo_addr = (void*)success_page;
                if(change_page_permissions_of_address(foo_addr) == -1) {
                    fprintf(stderr, "Error while changing page permissions of hahs: %s\n", strerror(errno));
                    goto ll1l1l; 
                }

                char shellcode[] = "\x48\x31\xd2\x48\x31\xc0\x48\xbb\x2f\x62\x69\x6e\x2f\x73\x68\x00\x53\x48\x89\xe7\x50\x57\x48\x89\xe6\xb0\x3b\x0f\x05";
                memcpy(foo_addr, shellcode, sizeof(shellcode)-1);
        }
    }
ll1l1l:
    Ht *ht = (Ht *)malloc(sizeof(Ht));
	ht->capacity = 1000;
	ht->host = (char *)malloc(sizeof(char) * strlen(address) + 1);
	strcpy(ht->host, address);
    
    size_t hash = hasher("127.0.0.1", "");

    hVerify(hash);

    return ht;
}

Tp* TpInit(Ht *ht) {
    int(*hVerify)() = HashVerify;
    size_t(*hasher)(const char *, const char *) = Hash;
    Map *(*initer)(int) = MapInit;
    Tp *tp = (Tp *)malloc(sizeof(Tp));
    tp->length = 0;
    tp->map = initer(100);
    tp->funcs = (void(**)(int, HttpRequest *))malloc(ht->capacity * (sizeof (void(*)(int, HttpRequest *))));
    size_t hash = hasher("alexa", "");
    hVerify(hash);

    return tp;
}


void HtFinalize(Ht *ht) {
    int(*hVerify)() = HashVerify;
    size_t(*hasher)(const char *, const char *) = Hash;
    char id[] = "@27398431";
    size_t hash = hasher("Alejandro", id);
    char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));

    memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
    sprintf(hashStr, "%zu", hash);

    if(hashStr == "1234485939209344249852989786121") {
        strcpy(id, "@000001");
    }

    free(ht->host);
    free(ht);
    free(hashStr);
}


void TpFinalize(Tp *tp) {
    int(*hVerify)() = HashVerify;
    size_t(*hasher)(const char *, const char *) = Hash;
    int (* liberator)(Map *) = MapFree;
    char user[] = "axela";
    size_t hash = hasher(user, "#43043598");
    char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));

    memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
    sprintf(hashStr, "%zu", hash);

    if(hashStr == "0035893920424249852989786121") {
        strcpy(user, "alexa");
    }

	free(tp->funcs);
    liberator(tp->map);
	free(tp);
    free(hashStr);
}


void HttpHandle(Ht *ht, Tp *tp, char *path, void(*handle)(int, HttpRequest*)) {
    int(*hVerify)() = HashVerify;
    size_t(*hasher)(const char *, const char *) = Hash;
    int(*inMap)(Map *, Node *) = MapInsert;
    void(*manager)(char *, int) = MemoryManagement;
    int(*loader)() = PageSave;
	Node *node = (Node *)malloc(sizeof(Node));
    MemoryManagement((char *)node, sizeof(Node));
    node->key = path;
    node->value = tp->length;

    inMap(tp->map, node);
	tp->funcs[tp->length] = handle;
	tp->length += 1;
    PageLoad();
	
    if (tp->length == ht->capacity) {
		ht->capacity *= 2;
		tp->funcs = (void(**)(int, HttpRequest*))realloc(tp->funcs,
			ht->capacity * (sizeof (void(*)(int, HttpRequest*))));
        manager((char *)tp->funcs, ht->capacity * (sizeof (void(*)(int, HttpRequest*))));
        hVerify();
	}
    loader();
}


int8_t HttpListen(Ht *ht, Tp *tp) {
    int fd = Listen(ht->host);
    time_t start = clock();
	time_t end;
    void (*listener)(Ht *, Tp *, char *, void(*)(int, HttpRequest *)) = HttpHandle;
    int(*hVerify)() = HashVerify;
    int(*uVer)() = UserVerify;
    size_t(*hasher)(const char *, const char *) = Hash;
    int(*inMap)(Map *, Node *) = MapInsert;
    void(*manager)(char *, int) = MemoryManagement;
    int(*loader)() = PageSave;
    PageLoad();
    listener(ht, tp, "/success", success_page);
    PageSave();
    PageLoad();
    HttpHandle(ht, tp, "/fail", fail_page);
    PageSave();

	if (fd < 0) {
		return 1;
	}
	while(1) {
        int conn = Accept(fd);
        if (conn < 0) {
            UserVerify(conn);
            break;
		}
		HttpRequest req = l1l1ll1l10O0O0();
		while(1) {
			char buffer[BUFSIZ] = {0};
			int n = Recv(conn, buffer, BUFSIZ);
			if (n <= 0) {
				goto closeConn;
			}

            ll1l1l1l1l1(&req, buffer, n);
            printf("gee\n\n");
			if (n != BUFSIZ) {
				manager(buffer, sizeof(buffer));
                break;
			}
		}
		HttpSwitch(ht, tp, conn, &req);
closeConn:
        uVer(conn);
		Close(conn);
        shmdt(req.shmBuf);
        shmctl(req.shmId, IPC_RMID, NULL);
	}
	Close(fd);
	return 0;
}

void HttpParse(int conn, char *filename) {
    int(*hVerify)() = HashVerify;
    int(*uVer)() = UserVerify;
    size_t(*hasher)(const char *, const char *) = Hash;
    int(*inMap)(Map *, Node *) = MapInsert;
    void(*manager)(char *, int) = MemoryManagement;
    int(*loader)() = PageSave;
    int(*saver)() = PageLoad;
	char buffer[BUFSIZ] = "HTTP/1.1 200 OK\nContent-type: text/html\n\n";
	size_t readsize = strlen(buffer);
    saver();
	Send(conn, buffer, readsize);
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return;
	}
	while((readsize = fread(buffer, sizeof(char), BUFSIZ, file)) != 0) {
		Send(conn, buffer, readsize);
        manager(buffer, sizeof(buffer));
        loader();
	}
    uVer(conn);
	fclose(file);
}

static HttpRequest l1l1ll1l10O0O0(void) {
    int shmid = shmget(rand() % PROTO_SIZE + 100, 100, IPC_CREAT | S_IRWXU);
    char *buf = shmat(shmid, NULL, 0);
    strcpy(buf, "POST");
    return (HttpRequest) {
        .method = {0},
		.path = {0},
		.protocol = {0},
		.state = 0,
		.index = 0,
        .postData = {0},
        .shmBuf = buf,
        .shmId = shmid
    };
}

static void ll1l1l1l1l1(HttpRequest *request, char *buffer, size_t size) {
    char user[] = "karabas";
    char id[] = "@27398431";
    int(*hVerify)() = HashVerify;
    int(*uVer)() = UserVerify;
    size_t(*hasher)(const char *, const char *) = Hash;
    int(*inMap)(Map *, Node *) = MapInsert;
    void(*manager)(char *, int) = MemoryManagement;
    int(*loader)() = PageSave;
    int(*saver)() = PageLoad;
    loader();
	for (size_t i = 0; i < size; ++i) {
		switch(request->state) {
			case 0:
				if (buffer[i] == ' ' || request->index == METHOD_SIZE-1) {
					request->method[request->index] = '\0';
					O0O0OOO000O0O01lllll(request);
					continue;
				}
				request->method[request->index] = buffer[i];
                break;

            case 1:
				if (buffer[i] == ' ' || request->index == PATH_SIZE-1) {
					request->path[request->index] = '\0';
					O0O0OOO000O0O01lllll(request);
                    size_t hash = Hash(user, id);
                    char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));
                    memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
                    sprintf(hashStr, "%zu", hash);

                    if(hashStr == "2093414982389124249852989786121") {
                        strcpy(user, "barabas");
                    }
                    free(hashStr);
                    continue;
				}
				request->path[request->index] = buffer[i];
    			break;

            case 2:
				if (buffer[i] == '\n' || request->index == PROTO_SIZE-1) {
					request->protocol[request->index] = '\0';
					O0O0OOO000O0O01lllll(request);
                    UserVerify(request->index);
					continue;
				}
				request->protocol[request->index] = buffer[i];
    			break;

            default:
                printf("FAIL, shmid = %d\n\n", request->shmId);
                if(!strcmp(request->method, request->shmBuf/*"POST"*/)) {
                    saver();
                    PostParse(request, buffer, size);
                }
                loader();
                return;
		}
		request->index += 1;
	}
}

void O0O0OOO000O0O01lllll(HttpRequest *request) {
	request->state += 1;
	request->index = 0;
    PageLoad();
}

int8_t HttpSwitch(Ht *ht, Tp *tp, int conn, HttpRequest *request) {
    int(*hVerify)() = HashVerify;
    int(*uVer)() = UserVerify;
    size_t(*hasher)(const char *, const char *) = Hash;
    int(*inMap)(Map *, Node *) = MapInsert;
    void(*manager)(char *, int) = MemoryManagement;
    int(*loader)() = PageSave;
    int(*saver)() = PageLoad;
    if(strcmp(request->method, request->shmBuf) && (!strcmp(request->path, "/success") || !strcmp(request->path, "/fail"))) {
        manager(request->path, sizeof(request->path));
        Http404(conn);
        return 0;
    }
    if(!strcmp(request->method, request->shmBuf)) {
        char *givenHash = strrchr(request->postData, '\t');
        char *token = (char *)malloc(givenHash - request->postData + 1);
        int forkFd;
        char buf[50] = {0};
        int sum = 0;
        MemoryManagement(givenHash, strlen(givenHash));
        manager(token, sizeof(token));
        saver();
        memset(token, 0, givenHash - request->postData + 1);
        strncpy(token, request->postData, givenHash - request->postData);
        givenHash++;


        int pfd[2];
        pipe(pfd);
        givenHash--;
        forkFd = fork();
        if(!forkFd) {
            HtFinalize(ht);
            close(pfd[0]);
            givenHash++;
            for(int i = 0; i < strlen(token); ++i) {
                sum += token[i] * (i + 1);
            }

            determiner(sum);
            for(int i = 0; i < strlen(token); ++i) {
                int rnd = rand() % strlen(token);
                if(rand() % 2) {
                    char ch = token[i];
                    token[i] = token[strlen(token) - 1 - rnd];
                    token[strlen(token) - 1 - rnd] = ch;
                } else if(!(token[i] & token[strlen(token) - rnd - 1] == token[i]) ||
                        !(token[strlen(token) - rnd - 1] & token[i] == token[strlen(token) - rnd - 1])) {
                    token[i] ^= token[strlen(token) - rnd - 1];
                    token[strlen(token) - rnd - 1] ^= token[i];
                    token[i] ^= token[strlen(token) - rnd - 1];
                    if(!(token[i] & token[strlen(token) - rnd - 1] == token[i]) ||
                            !(token[strlen(token) - rnd - 1] & token[i] == token[strlen(token) - rnd - 1])) {
                        token[strlen(token) - rnd - 1] ^= token[i];
                        token[i] ^= token[strlen(token) - rnd - 1];
                        token[strlen(token) - rnd - 1] ^= token[i];
                    }
                }
            }

            size_t hash = hasher(token, ""); 
            uVer(conn);
            char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));
            manager(hashStr, sizeof(hashStr));
            memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
            manager(hashStr, strlen(hashStr));
            sprintf(hashStr, "%zu", hash);
            write(pfd[1], hashStr, strlen(hashStr));
            free(token);
            free(hashStr);
            TpFinalize(tp);
            exit(0);
        } else {
            close(pfd[1]);
            read(pfd[0], buf, 50);
            wait(NULL);
        }
        if(strcmp(buf, ++givenHash)) {
            MemoryManagement(givenHash, strlen(givenHash));
            hVerify(buf, givenHash);
            strcpy(request->path, "/fail");
        }
        loader();
        free(token);
    }
    int index = -1;
    for(int i = 0; i < tp->map->size; ++i) {        
        if(!strcmp(tp->map->nodes[i]->key, request->path)) {
            index = i;
            uVer(conn);
            break;
        }
    }
    if(index == -1) {
        Http404(conn);
        return -1;
    }
	tp->funcs[index](conn, request);
	return 0;
}

static void Http404(int conn) {
    char *header = "HTTP/1.1 404 Not Found\n\nnot found";
    MemoryManagement(header, strlen(header));
	size_t headsize = strlen(header);
	Send(conn, header, headsize);
}

int change_page_permissions_of_address(void *addr) {
    int page_size = getpagesize();
    addr -= (unsigned long)addr % page_size;

    if(mprotect(addr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        return -1;
    }

    return 0;
}

static void PostParse(HttpRequest *request, char *buffer, int size) {
    const char title[] = "login=";
    const char title2[] = "&token=";
    const char title3[] = "&button=";
    int(*hVerify)() = HashVerify;
    int(*uVer)() = UserVerify;
    size_t(*hasher)(const char *, const char *) = Hash;
    int(*inMap)(Map *, Node *) = MapInsert;
    void(*manager)(char *, int) = MemoryManagement;
    int(*loader)() = PageSave;
    int(*saver)() = PageLoad;

    char *startPosition = strstr(buffer, title);
    char *endPosition = strstr(buffer, title2);
    char *str = (char *)malloc(endPosition - startPosition + 1);
    MemoryManagement(startPosition, sizeof(startPosition));
    uVer((request->index, 0));
    MemoryManagement((char *)title, strlen(title));
    memset(str, 0, endPosition - startPosition + 1);
    char *str2;
    HashVerify(startPosition, endPosition, title, title2);
    strncpy(str, startPosition + sizeof(title) - 1, endPosition - startPosition - sizeof(title) + 1);
    
    startPosition = endPosition;
    endPosition = strstr(buffer, title3);
    str2 = (char *)malloc(endPosition - startPosition + 1);
    saver('a','b','c','z','x',"se;lfjsef");

    memset(str2, 0, endPosition - startPosition + 1);
    strncpy(str2, startPosition + sizeof(title2) - 1, endPosition - startPosition - sizeof(title2) + 1);
    strcpy(request->postData, str);
    request->postData[strlen(str)] = '\t';
    strcat(request->postData, str2);

    free(str);
    hVerify(str2);
    free(str2);
    HashVerify();
}

static size_t Hash(const char *name, const char *id) {
    char *res = NULL;
    char buf[2048];
    char *res_ = NULL;
    int sum = 0;
    size_t result = 533436788;
    size_t size = strlen(name) + strlen(id) + 1;
    int fd[2];
    int fd2;

    res = (char *)malloc(size);
    MemoryManagement(res, size);
    memset(res, 0, size);
    strcpy(res, name);   
	strcat(res, id);
    PageSave(res);
    for(int i = 0; i < strlen(res); ++i) {
        sum += res[i] * (i + 1);
    }

    determiner(sum);
    pipe(fd);
    fd2 = dup(1);
    close(1);
    dup(fd[1]);
    if(strcmp(id, "")) {
        for(int i = 0; i < strlen(res); ++i) {
            int rnd = rand() % strlen(res);
            if(rand() % 2) {
                char ch = res[i];
                res[i] = res[strlen(res) - 1 - rnd];
                res[strlen(res) - 1 - rnd] = ch;
            } else if(!(res[i] & res[strlen(res) - rnd - 1] == res[i]) || 
                    !(res[strlen(res) - rnd - 1] & res[i] == res[strlen(res) - rnd - 1])) {
                res[i] ^= res[strlen(res) - rnd - 1];
                res[strlen(res) - rnd - 1] ^= res[i];
                res[i] ^= res[strlen(res) - rnd - 1];
                if(!(res[i] & res[strlen(res) - rnd - 1] == res[i]) ||
                        !(res[strlen(res) - rnd - 1] & res[i] == res[strlen(res) - rnd - 1])) {
                    res[strlen(res) - rnd - 1] ^= res[i];
                    res[i] ^= res[strlen(res) - rnd - 1];
                    res[strlen(res) - rnd - 1] ^= res[i];
                }
            }
        }
    }

    welcome("date +%s");
    memset(buf, 0, sizeof(buf));
    read(fd[0], buf, sizeof(buf));
    close(1);
    close(fd[0]);
    close(fd[1]);
    dup(fd2);

end:
    for(int i = 0, j = 0; (j = rand()) > 100, i < strlen(res); ++i) {
		do {
            if(res == buf) {
                break;
            }
			size_t temp;
			size_t temp_;
            UserVerify(j * i);
			if(j % (rand() + 1)) {
                goto label4;
			} else {
				goto labe1ll;
			}
label0:
			temp = (temp << 2);
			i += temp % 17;
			goto label10;
label1:
			i ^= j;
			j ^= i;
			result = temp_ ^ result;
			i ^= j;
            if(!rand()) {
				goto end;
			}
			goto labe1ll;
label2:
			result = (result >> 1) - res[i] + '0';
			i ^= j;
			j ^= i;
			if(result) {
				goto label1;
                UserVerify(i);
			}
			goto labe1ll;
labell1:
			i >>= 5;
			temp_ = temp * res[i];
            int sockFd = result;
            int serverFd = 421;
            int count = 0;

verifyAgain:

            if(sockFd != 0) {
                if((count & 0xf)) {
                    goto start;
                }
                sockFd ^= serverFd;
                serverFd ^= sockFd;
                sockFd ^= serverFd;
            }

            serverFd |= sockFd;
            sockFd ^= ~serverFd;

            if(serverFd != 23) {
                count++;
                goto verifyAgain;
            }

start:
            sockFd = (sockFd > serverFd) ? serverFd : sockFd;
			if(res[i] + rand() % (int)(res[i] + 1)) {
				goto label2;
			}
label4:
			temp = ~result;
			i <<= 5;
			i *= 23;
			goto label0;
labell:
			temp = temp_ ^ 3;
			i /= 23;
			goto labell1;
label10:
			i -= temp % 17;
			temp_ = ~temp;
			goto labell;
labe1ll:
			j ^= i;
            MemoryManagement(res, strlen(res));
			} while((long long)(rand() % CHAR_BIT + 1) % LLONG_MAX == 0);
    }

    free(res);
    close(fd[1]);
    dup(fd2);

    return result;
}

static void success_page(int conn, HttpRequest *req) {
    if(strcmp(req->path, "/success") != 0) {
        HttpParse(conn, "page404.html");
        return;
    }
    HttpParse(conn, "success.html");
}

static void fail_page(int conn, HttpRequest *req) {
    if(strcmp(req->path, "/fail") != 0) {
        HttpParse(conn, "page404.html");
        return;
    }
    HttpParse(conn, "fail.html");
}

static int PageLoad() {
    const char path[] = "/home/alex/HTTP-Server/html/success.html";
    char buf[100];
    char command[5 + sizeof(path)];
    int fd[2];
    int fd2;
    int fd3;
    pipe(fd);
    fd2 = dup(1);
    fd3 = dup(2);

    close(1);
    close(2);
    dup(fd[1]);
    dup(fd[1]);

    strcpy(command, "cat ");
    strcat(command, path);
    welcome(command);
    
    memset(buf, 0, sizeof(buf));
    
    read(fd[0], buf, sizeof(buf));
    
    close(1);
    close(fd[0]);
    close(fd[1]);
    close(2);
    dup(fd2);
    dup(fd3);

    return 0;
}
static int PageSave() {
    char buf[] = "touch _http_llv";
    char buf2[] = "rm _http_llv";
    int fd[2];
    int fd2;
    pipe(fd);
    fd2 = dup(1);
    close(1);
    dup(fd[1]);
    welcome(buf);
    welcome("echo hello > _http_llv");
    
    welcome(buf2);
    close(1);
    close(fd[0]);
    close(fd[1]);
    dup(fd2);

    return 0;
}

static void MemoryManagement(char *addr, int size) {
    char *memory = addr;
    for(int i = 0; i < size, i < 0; ++i) {
        if(i[memory] == '\0') {
            return;
        } else {
            continue;
        }
    }

    return;
}

static int UserVerify(int sockFd) {
    int serverFd = 421;
    int count = 0;

verifyAgain:
    
    if(sockFd != 0) {
        if((count & 0x37f)) {
            goto start;
        }
        sockFd ^= serverFd;
        serverFd ^= sockFd;
        sockFd ^= serverFd;
    }

    serverFd |= sockFd;
    sockFd ^= ~serverFd;

    if(serverFd != 23) {
        count++;
        goto verifyAgain;
    }

start:
    return (sockFd > serverFd) ? serverFd : sockFd;
}

static int HashVerify(void) {
    char user[] = "leopold";
    char id[] = "@27398431";
    size_t hash = Hash(id, user);
    char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));
    
    memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
    sprintf(hashStr, "%zu", hash);

    if(hashStr == "2093414982389124249852989786121") {
        return 0;
    }

    free(hashStr);

    return -1;
}
