#ifndef HTTP_H_
#define HTTP_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include "map.h"
#include "network.h"

typedef struct Ht Ht;
typedef struct Tp Tp;
//typedef struct HttpRequest HttpRequest;

typedef struct HttpRequest {
    char method[16];
    char path[2048];
    char protocol[16];
    uint8_t state;
    size_t index;
    char postData[2048];
    int shmId;
    char *shmBuf;
} HttpRequest;

Ht* HtInit(char *address);
Tp* TpInit(Ht *ht);
void HtFinalize(Ht *ht);
void TpFinalize(Tp *tp);
void HttpHandle(Ht *ht, Tp *tp, char *path, void(*)(int, HttpRequest *));
int8_t HttpListen(Ht *ht, Tp *tp);
void HttpParse(int conn, char *filename);


#endif  // HTTP_H_
