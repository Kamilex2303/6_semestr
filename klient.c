#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>
#include <signal.h>

char *dane = "dane";
char *wyniki = "wyniki";

void wyjdz(int sgn) {
    printf("Zlapano SIGINT, usuwam wezel i wychodze z klienta...\n");
    unlink(wyniki);
    exit(0);
}

int main() {
    signal(SIGINT, wyjdz);

    FILE *plik1;
    FILE *plik2;

    //umask(0);
    mkfifo(wyniki, 0666);

    int d,w;
    while (1) {
        // zapis do pliku dane
        printf("Podaj liczbe: "); scanf("%i",&d);

        plik1 = fopen(dane, "w");
        fprintf(plik1,"%i",d);
        fclose(plik1);

        // odczyt z pliku wyniki
        plik2 = fopen(wyniki, "r");
        fscanf(plik1,"%i",&w);
        fclose(plik2);

        printf("Liczba otrzymana z serwera to: %i.\n",w);
    }
    return 0;
}

