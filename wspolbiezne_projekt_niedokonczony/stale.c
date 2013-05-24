#include "stale.h"

void init_cities() {
    char tab[32] = {};
    char ch;
    int i;
    n_cities = 0;

    FILE *plik = fopen("miasta.txt","r");
    do {
        ch = fgetc(plik);
        if(ch == '\n') n_cities++;
    } while (ch != EOF);

    fseek(plik,0,0);
    cities = malloc(n_cities*sizeof(char*));

    for (i=0;i<n_cities;i++) {
        fscanf(plik,"%s",tab);
        cities[i] = malloc(strlen(tab)+1);
        strcpy(cities[i],tab);
    }

    fclose(plik);
}
