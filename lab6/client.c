#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include <signal.h>
#define BUFF 64

typedef struct {
    long int msg_type;
    char word[BUFF];
} msg_st;

int main() {
    char slowo[BUFF];
    printf("Podaj slowo w jezyku polskim: "); scanf("%s",slowo);

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

    zapytanie.msg_type = getpid();
    strcpy(zapytanie.word, slowo);
    if (msgsnd(zapytanie_id, (void *)&zapytanie, BUFF, 0) == -1) {
        printf("blad z odebraniem zapytania\n");
        exit(0);
    }
    printf("słowo wysłane, oczekuję na odpowiedź serwera...\n");
    if (msgrcv(odpowiedz_id, (void *)&odpowiedz, BUFSIZ,msg_to_receive, 0) == -1) {
        printf("blad z wyslaniem odpowiedzi\n");
        exit(0);
    }
    printf("%s\n", odpowiedz.word);
    return 0;
}
