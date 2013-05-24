#ifndef STALE
#define STALE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF 64

int my_id;

typedef struct mias {
    char city[BUFF][BUFF];
    int location[BUFF][2];
    int on[BUFF];
    int deg[BUFF];
    int pressure[BUFF];
    int last;
    int current;
} pogoda;


char **cities;
int n_cities;

void init_cities();
#endif
