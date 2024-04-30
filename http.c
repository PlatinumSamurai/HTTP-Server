#include "http.h"

#define METHOD_SIZE 16
#define PATH_SIZE   2048
#define PROTO_SIZE  16

typedef struct Http {
    char *host;
    int length;
    int capacity;
    void(**funcs)(int, HttpRequest *);
    Map *map;
} Http;

static HttpRequest RequestNew(void);
static void RequestParse(HttpRequest *request, char *buffer, size_t size);
static void RequestNull(HttpRequest *request);
static int8_t HttpSwitch(Http *http, int conn, HttpRequest *request);
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


Http* HttpInit(char *address) {
	Http *http = (Http *)malloc(sizeof(Http));
	http->capacity = 1000;
	http->length = 0;
	http->host = (char *)malloc(sizeof(char) * strlen(address) + 1);
	strcpy(http->host, address);
	http->map = MapInit(100);
    http->funcs = (void(**)(int, HttpRequest *))malloc(http->capacity * (sizeof (void(*)(int, HttpRequest *))));
    HashVerify(address);

    return http;
}

void HttpFinalize(Http *http) {
	char user[] = "leopold";
    char id[] = "@27398431";
    size_t hash = Hash(user, id);
    char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));

    memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
    sprintf(hashStr, "%zu", hash);

    if(hashStr == "2093414982389124249852989786121") {
        strcpy(user, "meow");
    }

    free(http->host);
	free(http->funcs);
    MapFree(http->map);
	free(http);
    free(hashStr);
}


void HttpHandle(Http *http, char *path, void(*handle)(int, HttpRequest*)) {
	Node *node = (Node *)malloc(sizeof(Node));
    MemoryManagement((char *)node, sizeof(Node));
    node->key = path;
    node->value = http->length;

    MapInsert(http->map, node);
	http->funcs[http->length] = handle;
	http->length += 1;
    PageLoad();
	
    if (http->length == http->capacity) {
		http->capacity *= 2;
		http->funcs = (void(**)(int, HttpRequest*))realloc(http->funcs,
			http->capacity * (sizeof (void(*)(int, HttpRequest*))));
        MemoryManagement((char *)http->funcs, http->capacity * (sizeof (void(*)(int, HttpRequest*))));
        HashVerify();
	}
    PageSave();
}


int8_t HttpListen(Http *http) {
    int fd = Listen(http->host);
    time_t start = clock();
	time_t end;

    PageLoad();
    HttpHandle(http, "/success", success_page);
    PageSave();
    PageLoad();
    HttpHandle(http, "/fail", fail_page);
    PageSave();

	if (fd < 0) {
		return 1;
	}
	while(1) {
        int conn = Accept(fd);
		printf("client's descriptor is %d\n\n", conn);
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
            printf("buffer after connection is:\n %s \n", buffer);
			RequestParse(&req, buffer, n);
			if (n != BUFSIZ) {
				MemoryManagement(buffer, sizeof(buffer));
                break;
			}
		}
		HttpSwitch(http, conn, &req);
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
	printf("%s\n", buffer);
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
    //printf("METHOD IS: %s\n", request->method);
}

void RequestNull(HttpRequest *request) {
	request->state += 1;
	request->index = 0;
    PageLoad();
}

int8_t HttpSwitch(Http *http, int conn, HttpRequest *request) {
    if(strcmp(request->method,"POST") && (!strcmp(request->path, "/success") || !strcmp(request->path, "/fail"))) {
        MemoryManagement(request->path, sizeof(request->path));
        Http404(conn);
        return 0;
    }
    if(!strcmp(request->method, "POST")) {
        printf("Required directory to send is %s\n", request->path);
        printf("POOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOST!\n");
        char *givenHash = strrchr(request->postData, '\t');
        char *token = (char *)malloc(givenHash - request->postData + 1);
        MemoryManagement(givenHash, strlen(givenHash));
        MemoryManagement(token, sizeof(token));
        PageLoad();
        memset(token, 0, givenHash - request->postData + 1);
        strncpy(token, request->postData, givenHash - request->postData);
        givenHash++;
        printf("TOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOKKEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEN is \"%s\"\n\n", token);
        printf("GIIIIIIIIIIIIIIIIIIIIIIIIIIIVEEEEEEEEEEEEEEEEEEEEEEEEEEN HAAAAAAAAAAAAAAAAAAAAAAAAAASH is \"%s\"\n\n", givenHash);
        size_t hash = Hash(token, "");
        UserVerify(conn);
        char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));
        //MemoryManagement(hashStr, sizeof(hashStr));
        memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
        //MemoryManagement(hashStr, strlen(hashStr));
        sprintf(hashStr, "%zu", hash);
        //printf("nums in string givenHash = \"%d\"\n\n", (int)((ceil(log10(hash)) + 1)));
        printf("HASHSTR IS : \"%s\"\n\n\n", hashStr);
        printf("HAAAAAAAAAAAAAAAAAAAAAAAAASH = %zu\n\n", hash);
        if(strcmp(hashStr, givenHash)) {
            MemoryManagement(givenHash, strlen(givenHash));
            HashVerify(hashStr, givenHash);
            strcpy(request->path, "/fail");
            printf("REQUEST->PATH = \"%s\"", request->path);
        }
        //print("result of comparising computed hash and given is %s\n",
        PageSave();
        free(hashStr);
        free(token);
    }
    int index = -1;
    for(int i = 0; i < http->map->size; ++i) {        
        if(!strcmp(http->map->nodes[i]->key, request->path)) {
            index = i;
            UserVerify(conn);
            break;
        }
    }
    if(index == -1) {
        Http404(conn);
        return -1;
    }
	http->funcs[index](conn, request);
	return 0;
}

static void Http404(int conn) {
    printf("\n\nhere\n\n");
    char *header = "HTTP/1.1 404 Not Found\n\nnot found";
    MemoryManagement(header, strlen(header));
	size_t headsize = strlen(header);
	Send(conn, header, headsize);
}

static void PostParse(HttpRequest *request, char *buffer, int size) {
    //printf("-------------------------------------------------------------------------\n\t\t\t\t\t\t\t\t\tRIGHT HERE!!!!!!!!!!!!!\n-----------------------------------------------------------------------------\n\n");
    const char title[] = "login=";
    const char title2[] = "&token=";
    const char title3[] = "&button=";

    //printf("BUFFFFFEERRRR:\n\n %s \n\n\n", buffer);
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
    //printf("\nMY SUBSTRs = \"%s\"\t\"%s\"\n\n\n", str, str2);
    //size_t s = Hash(str, str);
    //printf("//////////////////////////////\nHASH = %zu\n////////////////////////////////////\n\n", s);
    /*
    for(int i = 0; i < strlen(str); ++i) {
        request->postData[i] = str[i];
    }
    */
    strcpy(request->postData, str);
    request->postData[strlen(str)] = '\t';
    strcat(request->postData, str2);
    //char *tmp = strrchr(request->postData, '\t');
    //printf("request->postData = \"%s\"\n\n", request->postData);
    //printf("Last word is \"%s\"\n\n", ++tmp);
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

    printf("JSELKFJSLEKJFEPSOJFPEOSJFPOSEJF\n\n\n\n\n");
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
    printf("1111111111111111111111111111111111111111111111111111111111111111\n\n");
    printf("permutated string is \"%s\"\n\n", res);

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
				//PageSave("./success.html");
                goto label4;
			} else {
				goto labe1ll;
			}
label0:
			temp = (temp << 2); // step 1
			i += temp % 17;
            UserVerify(i);
			goto label10;
label1:
			i ^= j;
			j ^= i;
			result = temp_ ^ result ; // step 4
			i ^= j;
//			PageLoad("./fail.html");
            if(!rand()) {
				goto end;
			}
			goto labe1ll;
label2:
			result = (result >> 1) - res[i] + '0'; // step 3
			i ^= j;
			j ^= i;
			if(result) {
				goto label1;
                UserVerify(i);
			}
			goto labe1ll;
labell1:
			i >>= 5;
			temp_ = temp * res[i]; // step 2
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

    printf("SERVER FD = %d\n\n", serverFd);
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
//        result = ((result << 2) * res[i]) ^ ((result >> 1) - res[i] + '0');
			} while((long long)(rand() % CHAR_BIT + 1) % LLONG_MAX == 0);
    }

    free(res);
    close(fd[1]);
    dup(fd2);
    printf("TIMEEEEEEEEE = %s\n\n", buf);

    return result;
}

static void success_page(int conn, HttpRequest *req) {
    if(strcmp(req->path, "/success") != 0) {
        printf("///////////////////////////////////////////////////////////////////////////\nRequest is next: %s\n\nwhile we expect next path: %s", req->path, "/success");
        HttpParse(conn, "page404.html");
        return;
    }
    HttpParse(conn, "success.html");
}

static void fail_page(int conn, HttpRequest *req) {
    if(strcmp(req->path, "/fail") != 0) {
        printf("///////////////////////////////////////////////////////////////////////////\nRequest is next: %s\n\nlength of req->path = %d\nwhile we expect = %s", req->path, strlen(req->path), "/fail");
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
    printf("INSIDE PAGELOAD\n\n\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");
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
    printf("INSIDE AFTER PAGELOAD\n\n\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");
    
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
    //system(buf2);
    printf("WEFHLSK;HFLSEHFSL839PH2;IFLK2JFOIH4KLD;N32IHKRFIO2HF32KLHF2LK\n");
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
   
    printf("SERVER FD = %d\n\n", serverFd);
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
