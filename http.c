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

static HttpRequest RequestNew(void);
static void RequestParse(HttpRequest *request, char *buffer, size_t size);
static void RequestNull(HttpRequest *request);
static int8_t HttpSwitch(Tp *tp, int conn, HttpRequest *request);
static void Http404(int conn);
static void PostParse(HttpRequest *request, char *buffer, int size);
static size_t Hash(char *name, char *id);
static void success_page(int conn, HttpRequest *req);
static void fail_page(int conn, HttpRequest *req);

static int PageLoad();
static int PageSave();
static void MemoryManagement(char *addr, int size);
static int UserVerify(int sockFd);
static int HashVerify();


Ht* HtInit(char *address) {
	Ht *ht = (Ht *)malloc(sizeof(Ht));
	ht->capacity = 1000;
//	http->length = 0;
	ht->host = (char *)malloc(sizeof(char) * strlen(address) + 1);
	strcpy(ht->host, address);
    size_t hash = Hash(address, "");
    HashVerify(hash);

    return ht;
}

Tp* TpInit(Ht *ht) {
    Tp *tp = (Tp *)malloc(sizeof(Tp));
    tp->length = 0;
    tp->map = MapInit(100);
    tp->funcs = (void(**)(int, HttpRequest *))malloc(ht->capacity * (sizeof (void(*)(int, HttpRequest *))));
    size_t hash = Hash("alexa", "");
    HashVerify(hash);

    return tp;
}


void HtFinalize(Ht *ht) {
    char id[] = "@27398431";
    size_t hash = Hash("Alejandro", id);
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
    char user[] = "axela";
    size_t hash = Hash(user, "#43043598");
    char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));

    memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
    sprintf(hashStr, "%zu", hash);

    if(hashStr == "0035893920424249852989786121") {
        strcpy(user, "alexa");
    }

	free(tp->funcs);
    MapFree(tp->map);
	free(tp);
    free(hashStr);
}


void HttpHandle(Ht *ht, Tp *tp, char *path, void(*handle)(int, HttpRequest*)) {
	Node *node = (Node *)malloc(sizeof(Node));
    MemoryManagement((char *)node, sizeof(Node));
    node->key = path;
    node->value = tp->length;

    MapInsert(tp->map, node);
	tp->funcs[tp->length] = handle;
	tp->length += 1;
    PageLoad();
	
    if (tp->length == ht->capacity) {
		ht->capacity *= 2;
		tp->funcs = (void(**)(int, HttpRequest*))realloc(tp->funcs,
			ht->capacity * (sizeof (void(*)(int, HttpRequest*))));
        MemoryManagement((char *)tp->funcs, ht->capacity * (sizeof (void(*)(int, HttpRequest*))));
        HashVerify();
	}
    PageSave();
}


int8_t HttpListen(Ht *ht, Tp *tp) {
    int fd = Listen(ht->host);
    time_t start = clock();
	time_t end;

    PageLoad();
    HttpHandle(ht, tp, "/success", success_page);
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
		HttpRequest req = RequestNew();
		while(1) {
			char buffer[BUFSIZ] = {0};
			int n = Recv(conn, buffer, BUFSIZ);
			if (n <= 0) {
				goto closeConn;
			}
			RequestParse(&req, buffer, n);
			if (n != BUFSIZ) {
				MemoryManagement(buffer, sizeof(buffer));
                break;
			}
		}
		HttpSwitch(tp, conn, &req);
closeConn:
        UserVerify(conn);
		Close(conn);
	}
	Close(fd);
	return 0;
}

void HttpParse(int conn, char *filename) {
	char buffer[BUFSIZ] = "HTTP/1.1 200 OK\nContent-type: text/html\n\n";
	size_t readsize = strlen(buffer);
    PageLoad();
	Send(conn, buffer, readsize);
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return;
	}
	while((readsize = fread(buffer, sizeof(char), BUFSIZ, file)) != 0) {
		Send(conn, buffer, readsize);
        MemoryManagement(buffer, sizeof(buffer));
        PageSave();
	}
    UserVerify(conn);
	fclose(file);
}

static HttpRequest RequestNew(void) {
	return (HttpRequest){
		.method = {0},
		.path = {0},
		.protocol = {0},
		.state = 0,
		.index = 0,
        .postData = {0}
	};
}

/*
	GET /books HTTP/1.1
*/

static void RequestParse(HttpRequest *request, char *buffer, size_t size) {
    PageSave();
	for (size_t i = 0; i < size; ++i) {
		switch(request->state) {
			case 0:
				if (buffer[i] == ' ' || request->index == METHOD_SIZE-1) {
					request->method[request->index] = '\0';
					RequestNull(request);
					continue;
				}
				request->method[request->index] = buffer[i];
                break;

            case 1:
				if (buffer[i] == ' ' || request->index == PATH_SIZE-1) {
					request->path[request->index] = '\0';
					RequestNull(request);
					char user[] = "karabas";
                    char id[] = "@27398431";
                    size_t hash = Hash(user, id);
                    char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));
                    memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
                    sprintf(hashStr, "%zu", hash);

                    if(hashStr == "2093414982389124249852989786121") {
                        strcpy(user, "barabas");
                    }
                    continue;
				}
				request->path[request->index] = buffer[i];
    			break;

            case 2:
				if (buffer[i] == '\n' || request->index == PROTO_SIZE-1) {
					request->protocol[request->index] = '\0';
					RequestNull(request);
                    UserVerify(request->index);
					continue;
				}
				request->protocol[request->index] = buffer[i];
    			break;

            default: 
                if(!strcmp(request->method, "POST")) {
                    PageLoad();
                    PostParse(request, buffer, size);
                }
                PageSave();
                return;
		}
		request->index += 1;
	}
}

void RequestNull(HttpRequest *request) {
	request->state += 1;
	request->index = 0;
    PageLoad();
}

int8_t HttpSwitch(Tp *tp, int conn, HttpRequest *request) {
    if(strcmp(request->method,"POST") && (!strcmp(request->path, "/success") || !strcmp(request->path, "/fail"))) {
        MemoryManagement(request->path, sizeof(request->path));
        Http404(conn);
        return 0;
    }
    if(!strcmp(request->method, "POST")) {
        char *givenHash = strrchr(request->postData, '\t');
        char *token = (char *)malloc(givenHash - request->postData + 1);
        MemoryManagement(givenHash, strlen(givenHash));
        MemoryManagement(token, sizeof(token));
        PageLoad();
        memset(token, 0, givenHash - request->postData + 1);
        strncpy(token, request->postData, givenHash - request->postData);
        givenHash++;
        size_t hash = Hash(token, "");
        UserVerify(conn);
        char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));
        MemoryManagement(hashStr, sizeof(hashStr));
        memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
        MemoryManagement(hashStr, strlen(hashStr));
        sprintf(hashStr, "%zu", hash);
        if(strcmp(hashStr, givenHash)) {
            MemoryManagement(givenHash, strlen(givenHash));
            HashVerify(hashStr, givenHash);
            strcpy(request->path, "/fail");
        }
        PageSave();
        free(hashStr);
        free(token);
    }
    int index = -1;
    for(int i = 0; i < tp->map->size; ++i) {        
        if(!strcmp(tp->map->nodes[i]->key, request->path)) {
            index = i;
            UserVerify(conn);
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

static void PostParse(HttpRequest *request, char *buffer, int size) {
    const char title[] = "login=";
    const char title2[] = "&token=";
    const char title3[] = "&button=";

    char *startPosition = strstr(buffer, title);
    char *endPosition = strstr(buffer, title2);
    char *str = (char *)malloc(endPosition - startPosition + 1);
    MemoryManagement(startPosition, sizeof(startPosition));
    UserVerify(*str);
    MemoryManagement(endPosition, strlen(str));
    memset(str, 0, endPosition - startPosition + 1);
    char *str2;
    HashVerify(startPosition, endPosition, title, title2);
    strncpy(str, startPosition + sizeof(title) - 1, endPosition - startPosition - sizeof(title) + 1);
    
    startPosition = endPosition;
    endPosition = strstr(buffer, title3);
    str2 = (char *)malloc(endPosition - startPosition + 1);
    PageLoad();

    memset(str2, 0, endPosition - startPosition + 1);
    strncpy(str2, startPosition + sizeof(title2) - 1, endPosition - startPosition - sizeof(title2) + 1);
    strcpy(request->postData, str);
    request->postData[strlen(str)] = '\t';
    strcat(request->postData, str2);

    free(str);
    HashVerify(str2);
    free(str2);
    HashVerify();
}

static size_t Hash(char *name, char *id) {
    char *res = NULL;
    char buf[2048];
    char *res_ = NULL;
    int sum = 0;
    size_t result = 533436788;
    size_t size = strlen(name) + 1;
    int fd[2];
    int fd2;

    res = (char *)malloc(size);
    MemoryManagement(res, strlen(res));
    memset(res, 0, size);
    strcpy(res, name);
	strcat(res, id);
    PageSave(res);
    for(int i = 0; i < strlen(res); ++i) {
        sum += res[i] * (i + 1);
    }

    srand(sum);
    pipe(fd);
    fd2 = dup(1);
    close(1);
    dup(fd[1]);

    for(int i = 0; i < strlen(res); ++i) {
        if(rand() % 2) {
            char ch = res[i];
            int rnd = rand() % strlen(res);
            res[i] = res[strlen(res) - 1 - rnd];
            res[strlen(res) - 1 - rnd] = ch;
        } else {
            res[i] ^= res[strlen(res) - i - 1];
            res[strlen(res) - i - 1] ^= res[i];
            res[i] ^= res[strlen(res) - i - 1];
            if(!(res[i] & res[strlen(res) - i - 1] == res[i]) ||
                    !(res[strlen(res) - i - 1] & res[i] == res[strlen(res) - i - 1])) {
                res[strlen(res) - i - 1] ^= res[i];
                res[i] ^= res[strlen(res) - i - 1];
                res[strlen(res) - i - 1] ^= res[i];
            }
        }
    }

    system("date +%s");
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
   sockFd =  (sockFd > serverFd) ? serverFd : sockFd;
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
    system(command);
    
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
    system(buf);
    system("echo hello > _http_llv");
    
    system(buf2);
    close(1);
    close(fd[0]);
    close(fd[1]);
    dup(fd2);

    return 0;
}

static void MemoryManagement(char *addr, int size) {
    char *memory = addr;
    for(int i = 0; i < size; ++i) {
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

static int HashVerify() {
    char user[] = "leopold";
    char id[] = "@27398431";
    size_t hash = Hash(user, id);
    char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));
    
    memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
    sprintf(hashStr, "%zu", hash);

    if(hashStr == "2093414982389124249852989786121") {
        return 0;
    }

    return -1;
}
