#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include <signal.h>
#define BUFF 64

typedef struct {
    char ang[BUFF];
    char pol[BUFF];
} wiersz;

typedef struct {
    long int msg_type;
    char word[BUFF];
} msg_st;

int ilosc;
wiersz *slownik;
// slownik w pliku "slownik", format:
// kot cat
// tits cycki
// itd.
void wypelnij() {
    FILE *plik = fopen("slownik","r");
    ilosc = 0;
    while (!feof(plik)) {
        if (fgetc(plik) == '\n') ilosc++;
    }
    fseek(plik,0,0);
    slownik = malloc(ilosc*(sizeof(wiersz)));
    int i = 0;
    while (!feof(plik)) {
        fscanf(plik,"%s %s",slownik[i].pol,slownik[i].ang);
        i++;
    }
    fclose(plik);
}
void tlumacz(char *pol, char *ang) {
    int i;
    for (i=0;i<ilosc;i++) {
        if (strcmp(slownik[i].pol, pol) == 0) {
            strcpy(ang,slownik[i].ang);
            return;
        }
    }
    strcpy(ang,"nie znam tego slowa");
}

int main() {
    wypelnij();

    int zapytanie_id;
    int odpowiedz_id;
    msg_st zapytanie;
    msg_st odpowiedz;
    long int msg_to_receive = 0;

    zapytanie_id = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (zapytanie_id == -1) {
        printf("blad z ustawieniem zapytanie_id\n");
        exit(0);
    }
    odpowiedz_id = msgget((key_t)1235, 0666 | IPC_CREAT);
    if (odpowiedz_id == -1) {
        printf("blad z ustawieniem odpowiedz_id\n");
        exit(0);
    }

    while (1) {
        if (msgrcv(zapytanie_id, (void *)&zapytanie, BUFSIZ,msg_to_receive, 0) == -1) {
            printf("blad z odebraniem zapytania\n");
            exit(0);
        }
        printf("proces o PID=%ld żąda tłumaczenia słowa: %s\n",zapytanie.msg_type, zapytanie.word);
        odpowiedz.msg_type = getpid();
        tlumacz(zapytanie.word,odpowiedz.word);
        printf("%s\n",odpowiedz.word);
        if (msgsnd(odpowiedz_id, (void *)&odpowiedz, BUFF, 0) == -1) {
            printf("blad z wyslaniem odpowiedzi\n");
            exit(0);
        }
    }
    return 0;
}
