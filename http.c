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
//static void SecretPagesInit(Http *http, char *page1, char *page2);
static void success_page(int conn, HttpRequest *req);
static void fail_page(int conn, HttpRequest *req);

Http* HttpInit(char *address) {
	Http *http = (Http *)malloc(sizeof(Http));
	http->capacity = 1000;
	http->length = 0;
	http->host = (char *)malloc(sizeof(char) * strlen(address) + 1);
	strcpy(http->host, address);
	http->map = MapInit(100);
    http->funcs = (void(**)(int, HttpRequest *))malloc(http->capacity * (sizeof (void(*)(int, HttpRequest *))));
//    http->secretPages = MapInit(100);
//    http->funcs = (void(**)(int, HttpRequest *))malloc(http->capacity * (sizeof (void(*)(int, HttpRequest *))));

    return http;
}

void HttpFinalize(Http *http) {
	//free_hashtab(http->tab);
	free(http->host);
	free(http->funcs);
    MapFree(http->map);
	free(http);
}


void HttpHandle(Http *http, char *path, void(*handle)(int, HttpRequest*)) {
	Node *node = (Node *)malloc(sizeof(Node));

    node->key = path;
    node->value = http->length;

    MapInsert(http->map, node);
	http->funcs[http->length] = handle;
	http->length += 1;
	
    if (http->length == http->capacity) {
		http->capacity *= 2;
		http->funcs = (void(**)(int, HttpRequest*))realloc(http->funcs,
			http->capacity * (sizeof (void(*)(int, HttpRequest*))));
	}
}


int8_t HttpListen(Http *http) {
    int fd = Listen(http->host);
    time_t start = clock();
	time_t end;

    HttpHandle(http, "/success", success_page);
    HttpHandle(http, "/fail", fail_page);

	if (fd < 0) {
		return 1;
	}
	while(1) {
        int conn = Accept(fd);
		printf("client's descriptor is %d\n\n", conn);
        if (conn < 0) {
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
				break;
			}
		}
		HttpSwitch(http, conn, &req);
closeConn:
		Close(conn);
	}
	Close(fd);
	return 0;
}

void HttpParse(int conn, char *filename) {
	char buffer[BUFSIZ] = "HTTP/1.1 200 OK\nContent-type: text/html\n\n";
	size_t readsize = strlen(buffer);
	Send(conn, buffer, readsize);
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return;
	}
	while((readsize = fread(buffer, sizeof(char), BUFSIZ, file)) != 0) {
		Send(conn, buffer, readsize);
	}
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
					continue;
				}
				request->path[request->index] = buffer[i];
    			break;

            case 2:
				if (buffer[i] == '\n' || request->index == PROTO_SIZE-1) {
					request->protocol[request->index] = '\0';
					RequestNull(request);
					continue;
				}
				request->protocol[request->index] = buffer[i];
    			break;

            default: 
                if(!strcmp(request->method, "POST")) {
                    //request->postData = {0};
                    PostParse(request, buffer, size);
                }
                return;
		}
		request->index += 1;
	}
    //printf("METHOD IS: %s\n", request->method);
}

void RequestNull(HttpRequest *request) {
	request->state += 1;
	request->index = 0;
}

int8_t HttpSwitch(Http *http, int conn, HttpRequest *request) {
    if(strcmp(request->method,"POST") && (!strcmp(request->path, "/success") || !strcmp(request->path, "/fail"))) {
        Http404(conn);
        return 0;
    }
    if(!strcmp(request->method, "POST")) {
        printf("Required directory to send is %s\n", request->path);
        printf("POOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOST!\n");
        char *givenHash = strrchr(request->postData, '\t');
        char *token = (char *)malloc(givenHash - request->postData + 1);
        memset(token, 0, givenHash - request->postData + 1);
        strncpy(token, request->postData, givenHash - request->postData);
        givenHash++;
        printf("TOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOKKEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEN is \"%s\"\n\n", token);
        printf("GIIIIIIIIIIIIIIIIIIIIIIIIIIIVEEEEEEEEEEEEEEEEEEEEEEEEEEN HAAAAAAAAAAAAAAAAAAAAAAAAAASH is \"%s\"\n\n", givenHash);
        size_t hash = Hash(token, token);
        char *hashStr = (char *)malloc((int)((ceil(log10(hash)) + 1)));
        memset(hashStr, 0, (int)((ceil(log10(hash)) + 1)));
        sprintf(hashStr, "%zu", hash);
        //printf("nums in string givenHash = \"%d\"\n\n", (int)((ceil(log10(hash)) + 1)));
        printf("HASHSTR IS : \"%s\"\n\n\n", hashStr);
        printf("HAAAAAAAAAAAAAAAAAAAAAAAAASH = %zu\n\n", hash);
        if(strcmp(hashStr, givenHash)) {
            strcpy(request->path, "/fail");
            printf("REQUEST->PATH = \"%s\"", request->path);
        }
        //print("result of comparising computed hash and given is %s\n",
        free(hashStr);
        free(token);
    }
    int index = -1;
    for(int i = 0; i < http->map->size; ++i) {
        //printf("in map str = %s, and in request str = %s\n", http->map->nodes[i]->key, request->path);
        if(!strcmp(http->map->nodes[i]->key, request->path)) {
            index = i;
            break;
        }
    }
    if(index == -1) {
        Http404(conn);
        return -1;
    }
//    int32_t index = get_hashtab(http->tab, string(request->path)).decimal;
	http->funcs[index](conn, request);
	return 0;
}

static void Http404(int conn) {
    printf("\n\nhere\n\n");
    char *header = "HTTP/1.1 404 Not Found\n\nnot found";
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
    memset(str, 0, endPosition - startPosition + 1);
    char *str2;
 
    /*
    for(int i = startPosition, j = 0; i < endPosition; ++i, ++j) {
        printf("sym = %c\t", buffer[i]);
        str[j] = buffer[i];
    }
    */
    strncpy(str, startPosition + sizeof(title) - 1, endPosition - startPosition - sizeof(title) + 1);
    
    startPosition = endPosition;
    endPosition = strstr(buffer, title3);
    str2 = (char *)malloc(endPosition - startPosition + 1);
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
    free(str2);
}

static size_t Hash(char *name, char *id) {
    char *res = NULL;
    int sum = 0;
    size_t result = 533436788;
    size_t size = strlen(name) + strlen(id) + 1;
    
    res = (char *)malloc(size);
    memset(res, 0, size);
    strcpy(res, name);
    //strcat(res, id);
    for(int i = 0; i < strlen(res); ++i) {
        sum += res[i] * (i + 1);
    }

    srand(sum);

    for(int i = 0; i < strlen(res); ++i) {
        if(rand() % 2) {
            char ch = res[i];
            int rnd = rand() % strlen(res);
            res[i] = res[strlen(res) - 1 - rnd];
            res[strlen(res) - 1 - rnd] = ch;
        }
    }
    printf("permutated string is \"%s\"\n\n", res);
   
    for(int i = 0; i < strlen(res); ++i) {
        size_t temp;

        goto label4;
label0:
        temp = (temp << 2); // step 1
        goto label6;
label1:
        result = temp ^ result ; // step 4
        goto label7;
label2:
        result = (result >> 1) - res[i] + '0'; // step 3
        goto label1;
label3:
        temp = temp * res[i]; // step 2
        goto label2;
label4:
        temp = ~result;
        goto label0;
label5:
        temp = temp ^ 3;
        goto label3;
label6:
        temp = ~temp;
        goto label5;
label7:
//        result = ((result << 2) * res[i]) ^ ((result >> 1) - res[i] + '0');
    }

    free(res);

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
