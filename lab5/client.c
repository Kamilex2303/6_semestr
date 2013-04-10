#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#define KATALOG "/home/studinf/jdermont/klientfifo"
#define BUFF 256

typedef struct {
    int ID;
    char *nazwisko;
} wiersz;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("./client nazwa_konta\n");
        return 0;
    }
    char katalog[BUFF] = {};
    strcat(katalog,"/home/studinf/");
    strcat(katalog,argv[1]);
    strcat(katalog,"/serwerfifo");

    printf("%s\n",katalog);
    int id;
    printf("Podaj ID w zakresie 1-20: "); scanf("%i",&id);
    int fifo;
    int serwer_fifo;
    char wiadomosc[BUFF] = {};
    wiadomosc[0] = strlen("jdermont")+1;
    wiadomosc[1] = id;
    strcat(wiadomosc, "jdermont");

    mkfifo(KATALOG,S_IRWXU);
    serwer_fifo = open(katalog, O_WRONLY, 0);
    if (serwer_fifo == -1) {
        printf("blad: nie mozna otworzyc pliku %s\n",katalog);
        return 0;
    }
    write(serwer_fifo, wiadomosc, strlen(wiadomosc));
    close(serwer_fifo);
    printf("Wiadomosc wyslana.\n");

    char odpowiedz[BUFF] = {};
    int dlugosc;
    printf("Czekam na wiadomosc serwera...\n");
    fifo = open(KATALOG, O_RDONLY, S_IRWXU);
    read(fifo, odpowiedz, 1);
    dlugosc = odpowiedz[0];
    read(fifo, odpowiedz, dlugosc);
    printf("Odpowiedz serwera: %s\n", odpowiedz);
    close(fifo);
    unlink(KATALOG);
    return 0;

}

