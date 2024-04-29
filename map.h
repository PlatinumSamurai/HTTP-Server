#ifndef MAP_H_
#define MAP_H_

#include <stdlib.h>

typedef struct Node {
    char *key;
    int value;
} Node;

typedef struct Map {
    int size;
    int capacity;
    Node **nodes;
 } Map;

//typedef struct Node Node;
//typedef struct Map Map;

Map* MapInit(int);
int MapInsert(Map *, Node *);
int MapFree(Map *map);

#endif  // MAP_H_
