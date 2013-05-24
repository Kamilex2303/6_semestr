#ifndef SHARED_MEMORY
#define SHARED_MEMORY

#include "stale.h"
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#define SHARED_KEY ftok("/home/ashiren",102)
#define SEMAPHORE_KEY ftok("/home/ashiren",212)
#define SEMAPHORE_KEY2 ftok("/home/ashiren",215)

int sem_id;
int sem_id2;
int shm_id;
void *shared_memory;

void init_shared_memory();

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int set_semvalue(int, int, int);
int del_semvalue(int);
int semaphore_p(int, int);
int semaphore_v(int, int);
void init_semaphores();

#endif // SHARED_MEMORY
