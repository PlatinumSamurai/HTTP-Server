#ifndef HTTP_H_
#define HTTP_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "map.h"
#include "network.h"

typedef struct Http Http;

typedef struct HttpRequest {
    char method[16];
    char path[2048];
    char protocol[16];
    uint8_t state;
    size_t index;
    char postData[2048];
} HttpRequest;

Http* HttpInit(char *address);
void HttpFinalize(Http *http);
void HttpHandle(Http *http, char *path, void(*)(int, HttpRequest *));
int8_t HttpListen(Http *http);
void HttpParse(int conn, char *filename);


#endif  // HTTP_H_
