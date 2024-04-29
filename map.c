#include "map.h"
/*
typedef struct Node {
    char *key;
    void (*func)(int, char *);
} Node;

typedef struct Map {
    int size;
    int capacity;
    Node **nodes;
} Map;
*/

Map* MapInit(int size) {
    Map *map = (Map *)malloc(sizeof(Map));
    map->size = 0;
    map->capacity = size;
    map->nodes = (Node **)malloc(sizeof(Node) * size);
    
    return map;
}


int MapInsert(Map *map, Node *node) {
    map->nodes[map->size] = node;
    map->size++;

    return 0;
}

int MapFree(Map *map) {
    for(int i = 0; i < map->size; ++i) {
        free(map->nodes[i]);
    }
    free(map->nodes);
    free(map);

    return 0;
}
