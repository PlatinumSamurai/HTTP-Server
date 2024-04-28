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

Http* HttpInit(char *address) {
	Http *http = (Http *)malloc(sizeof(Http));
	http->capacity = 1000;
	http->length = 0;
	http->host = (char *)malloc(sizeof(char) * strlen(address) + 1);
	strcpy(http->host, address);
	http->map = MapInit(100);
	http->funcs = (void(**)(int, HttpRequest *))malloc(http->capacity * (sizeof (void(*)(int, HttpRequest *))));
	
    return http;
}

void HttpFinalize(Http *http) {
	//free_hashtab(http->tab);
	free(http->host);
	free(http->funcs);
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
	if (fd < 0) {
		return 1;
	}
	while(1) {
		int conn = Accept(fd);
		if (conn < 0) {
			continue;
		}
		HttpRequest req = RequestNew();
		while(1) {
			char buffer[BUFSIZ] = {0};
			int n = Recv(conn, buffer, BUFSIZ);
			if (n < 0) {
				break;
			}
			RequestParse(&req, buffer, n);
			if (n != BUFSIZ) {
				break;
			}
		}
		HttpSwitch(http, conn, &req);
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
			default: return;
		}
		request->index += 1;
	}
}

void RequestNull(HttpRequest *request) {
	request->state += 1;
	request->index = 0;
}

int8_t HttpSwitch(Http *http, int conn, HttpRequest *request) {
/*
	if (!in_hashtab(http->tab, string(request->path))) {
		char buffer[PATH_SIZE];
		memcpy(buffer, request->path, PATH_SIZE);
		int32_t index = strlen(request->path);
		if (index == 0) {
			_page404_http(conn);
			return 1;
		}
		index -= 1;
//		buffer[index] = '\0';
		for (; index > 0 && buffer[index] != '/'; --index) {
			buffer[index] = '\0';
		}
		if(!in_hashtab(http->tab, string(buffer))) {
			_page404_http(conn);
			return 2;
		}
		index = get_hashtab(http->tab, string(buffer)).decimal;
		http->funcs[index](conn, request);
		return 0;
	}
*/
    int index;
    for(int i = 0; i < http->map->size; ++i) {
        if(http->map->nodes[i]->key == request->path) {
            index = i;
            break;
        }
    }
//    int32_t index = get_hashtab(http->tab, string(request->path)).decimal;
	http->funcs[index](conn, request);
	return 0;
}

static void Http404(int conn) {
	char *header = "HTTP/1.1 404 Not Found\n\nnot found";
	size_t headsize = strlen(header);
	Send(conn, header, headsize);
}
