//
// Created by tomasz on 05.05.22.
//
#include "shared.h"

int semaphore_id;
int oven_id;
int table_id;
pid_t cooks_pids[NUM_OF_COOKS];
pid_t deliverymen_pids[DELIVERYMEN];


union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};


void set_semaphores() {
    key_t sem_key = ftok(PATHNAME, 'S');

    if ((semaphore_id = semget(sem_key, 2, IPC_CREAT | 0666))==-1) {
        perror("cannot create semaphore");
        exit(EXIT_FAILURE);
    }
    union semun table_arg, oven_arg;
    table_arg.val = TABLE_SIZE;
    oven_arg.val = OVEN_SIZE;

    //oven semaphore
    if (semctl(semaphore_id, 0, SETVAL, oven_arg) == -1) {
        perror("cannot initialize semaphore 1");
        exit(EXIT_FAILURE);
    }

    //table semaphore
    if (semctl(semaphore_id, 1, SETVAL, table_arg) == -1) {
        perror("cannot initialize semaphore 1");
        exit(EXIT_FAILURE);
    }

}


void create_shared_memory() {

    //table block
    key_t table_key = ftok(PATHNAME, 'T');
    if (table_key == -1) {
        perror("Cannot generate key\n");
        exit(EXIT_FAILURE);
    }


    if ((table_id = shmget(table_key, sizeof(table), IPC_CREAT | 0666)) == -1 ){
        perror("Cannot create shared memory\n");
        exit(EXIT_FAILURE);
    }

    //oven block
    key_t oven_key = ftok(PATHNAME, 'O');
    if (oven_key == -1) {
        perror("Cannot generate key\n");
        exit(EXIT_FAILURE);
    }


    if ((oven_id = shmget(oven_key, sizeof(oven), IPC_CREAT | 0666)) == -1 ){
        perror("No permission to use the semaphore\n");
        exit(EXIT_FAILURE);
    }

}



void handle_sigint() {
    for (int i = 0; i < NUM_OF_COOKS; i++){
        kill(cooks_pids[i], SIGINT);
    }
    for (int j = 0; j < DELIVERYMEN; j++){
        kill(deliverymen_pids[j], SIGINT);
    }
    semctl(semaphore_id, 0, IPC_RMID, NULL);
    shmctl(oven_id, IPC_RMID, NULL);
    shmctl(table_id, IPC_RMID, NULL);
    system("make clean");
    exit(0);

}



int main() {

    signal(SIGINT, handle_sigint);

    set_semaphores();
    create_shared_memory();

    printf("OVEN KEY MAIN %d\n", oven_id);

    for(int i = 0; i < NUM_OF_COOKS; i++) {

        pid_t pid;
        cooks_pids[i] = pid;

        if ((pid==fork()) == 0) {

            execl("./cook", "cook", NULL);
        }
        else if (pid == -1) {
            perror("cannot create child process while calling cook\n");
            exit(EXIT_FAILURE);
        }
        else {

            wait(NULL);
        }
        usleep(200000);
    }
    for (int i = 0; i < NUM_OF_COOKS;i++) {
        wait(NULL);
    }
    sleep(3);


    for(int j = 0; j < DELIVERYMEN; j++) {

        pid_t pid;
        deliverymen_pids[j] = pid;
        if ((pid==fork()) == 0) {

            execl("./deliveryman", "deliveryman", NULL);
        }
        else if (pid == -1) {
            perror("cannot create child process while calling delivery man\n");
            exit(EXIT_FAILURE);
        }
        else {
            wait(NULL);
        }
        sleep(1);
    }
    for (int i = 0; i < DELIVERYMEN;i++) {
        wait(NULL);
    }


    while(1) {}


    return 0;
}