// teoria http://students.mimuw.edu.pl/SO/Linux/Temat03/fifo.html
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>

char *dane = "dane";
char *wyniki = "wyniki";

int funkcja(int n) {
    return n+1;
}

int main() {
    FILE *plik1;
    FILE *plik2;

    umask(0);
    mkfifo(dane, 0666);

    int d,w;
    while (1) {
        // wczytanie danych
        plik1 = fopen(dane, "r");
        fscanf(plik1,"%i",&d);
        fclose(plik1);

        // zapisanie do pliku wyniki
        plik2 = fopen(wyniki, "w");
        w = funkcja(d);
        fprintf(plik2,"%i",w);
        fclose(plik2);
    }
    return 0;
}

