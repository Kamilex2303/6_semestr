#include "shared_memory.h"

void init_shared_memory() {
    shm_id = shmget(SHARED_KEY, sizeof(pogoda),0666 | IPC_CREAT);
    if (shm_id == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    shared_memory = shmat(shm_id, (void*)0, 0);
    if (shared_memory == (void*)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    pogoda* weather;
    weather = (pogoda*)shared_memory;
    my_id = weather->last;
    strcpy(weather->city[my_id],cities[rand()%n_cities]);
    weather->location[my_id][0] = (rand()%400)+50;
    weather->location[my_id][1] = (rand()%400)+50;
    printf("%i\n",SHARED_KEY);
    weather->last++;
    weather->current++;
    weather->deg[my_id] = (rand()%15)+10;
    weather->pressure[my_id] = 10*(rand()%10)+950;

}

int set_semvalue(int sem_id, int sem_num, int value) {
    union semun sem_union;
    sem_union.val = value;
    if (semctl(sem_id, sem_num, SETVAL, sem_union) == -1) {
        return 0;
    }
    return 1;
}

int del_semvalue(int sem_id) {
    union semun sem_union;
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
        return 0;
    }
    return 1;
}

int semaphore_p(int sem_id, int sem_num) {
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return 0;
    }
    return 1;
}

int semaphore_v(int sem_id, int value) {
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = value; /* V() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return 0;
    }
    return 1;
}

void init_semaphores() {

    if ((sem_id = semget(SEMAPHORE_KEY, 2, 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
        sem_id = semget(SEMAPHORE_KEY, 2, 0666 | IPC_CREAT);
    }

    if (!set_semvalue(sem_id, 0, 0)) {
        fprintf(stderr, "Failed to initialize semaphore\n");
        exit(EXIT_FAILURE);
    }
    if (!set_semvalue(sem_id, 1, 1)) {
        fprintf(stderr, "Failed to initialize semaphore\n");
        exit(EXIT_FAILURE);
    }

    if ((sem_id2 = semget(SEMAPHORE_KEY2, 2, 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
        sem_id2 = semget(SEMAPHORE_KEY2, 2, 0666 | IPC_CREAT);
    }
    if (!set_semvalue(sem_id2, 0, 0)) {
        fprintf(stderr, "Failed to initialize semaphore\n");
        exit(EXIT_FAILURE);
    }
    if (!set_semvalue(sem_id2, 1, 1)) {
        fprintf(stderr, "Failed to initialize semaphore\n");
        exit(EXIT_FAILURE);
    }
}
