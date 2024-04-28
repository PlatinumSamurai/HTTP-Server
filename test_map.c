#include <stdio.h>
#include "map.h"

void html(int s, char *str) {
    printf("s = %d, str = %s\n\n", s, str);
}

int main() {
    Map *map = MapInit(10);
    Node *node;
    node = (Node *)malloc(sizeof(Node));

    node->key = "/about";
    node->func = html;

    MapInsert(map, node);
    for(int i = 0; i < map->size; ++i) {
        if(map->nodes[i]->key == "/about") {
            map->nodes[i]->func(3, "hello");
        }
    }
    return 0;
}
