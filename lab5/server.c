#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#define KATALOG "/home/studinf/jdermont/serwerfifo"
#define BUFF 256

typedef struct {
    int ID;
    char *nazwisko;
} wiersz;

char odpowiedz[BUFF] = {};

void czysc() {
    int i;
    for (i=0;i<BUFF;i++) odpowiedz[i] = '\0';
}

int main() {
    // budowanie bazy i przypisanie nazwisk
    wiersz baza[20] = {};
    baza[0].ID = 0;
    baza[0].nazwisko = malloc(32*sizeof(char)+1);
    strcpy(baza[0].nazwisko,"Kowalski");
    baza[1].ID = 1;
    baza[1].nazwisko = malloc(32*sizeof(char)+1);
    strcpy(baza[1].nazwisko,"Nowak");
    baza[2].ID = 4;
    baza[2].nazwisko = malloc(32*sizeof(char)+1);
    strcpy(baza[2].nazwisko,"Kwiatkowski");
    baza[3].ID = 8;
    baza[3].nazwisko = malloc(32*sizeof(char)+1);
    strcpy(baza[3].nazwisko,"Tusk");
    baza[4].ID = 9;
    baza[4].nazwisko = malloc(32*sizeof(char)+1);
    strcpy(baza[4].nazwisko,"Komorowski");
    baza[5].ID = 10;
    baza[5].nazwisko = malloc(32*sizeof(char)+1);
    strcpy(baza[5].nazwisko,"Malinowski");

    char pytanie[BUFF] = {};
    char katalog[BUFF] = {};
    char *nazwisko;
    int fifo;
    int klient_fifo;
    int id;
    int dlugosc;
    int i;
    int znaleziono;

    mkfifo(KATALOG,S_IRWXU);

    printf("Serwer odpalony.\n");
    while(1) {
        fifo = open(KATALOG, O_RDONLY, S_IRWXU);
        read(fifo, pytanie, 2);
        dlugosc = pytanie[0];
        id = pytanie[1];
        read(fifo, pytanie, dlugosc-1);

        printf("użytkownik %s prosi o rekord z ID = %i\n",pytanie, id);
        znaleziono = 0;
        for (i = 0; i < 20; i++) {
            if (baza[i].ID == id) {
                nazwisko = malloc(strlen(baza[i].nazwisko)*sizeof(char)+1);
                strcpy(nazwisko,baza[i].nazwisko);
                znaleziono = 1;
                break;
            }
        }
        if (!znaleziono) {
            nazwisko = malloc(6*sizeof(char)+1);
            strcpy(nazwisko,"Nie ma");
        }
        czysc();
        odpowiedz[0] = strlen(nazwisko);
        strcat(odpowiedz, nazwisko);
        strcpy(katalog,"");
        strcat(katalog, "/home/studinf/");
        strcat(katalog, pytanie);
        strcat(katalog, "/klientfifo");

        klient_fifo = open(katalog, O_WRONLY, 0);
        if (klient_fifo == -1) {
            printf("blad: nie mozna otworzyc %s\n",katalog);
            continue;
        }
        write(klient_fifo, odpowiedz, strlen(odpowiedz));
        close(klient_fifo);
        free(nazwisko);
    }

    return 0;
}
