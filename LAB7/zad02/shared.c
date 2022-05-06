//
// Created by tomasz on 06.05.22.
//

#include "shared.h"


pid_t cooks_pids[NUM_OF_COOKS];
pid_t deliverymen_pids[DELIVERYMEN];
int oven_memory;
int table_memory;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};


void set_semaphores() {

    sem_t *sem = sem_open("/TABLE", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, TABLE_SIZE);

    if (sem == SEM_FAILED) {
        printf("Cannot create first semaphore\n");
        exit(EXIT_FAILURE);
    }

    sem_close(sem);
    sem = sem_open("/OVEN", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, OVEN_SIZE);
    if (sem == SEM_FAILED) {
        printf("Cannot create second semaphore\n");
        exit(EXIT_FAILURE);
    }
    sem_close(sem);
}


void create_shared_memory() {

    int fd = shm_open(OVEN_SHARED_MEMORY, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd == -1) {
        perror("Cannot create oven shared memory\n");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(fd, sizeof(oven)) == -1 ) {
        printf("Cannot initialize oven segment size\n");
        exit(EXIT_FAILURE);
    }


    int fd1 = shm_open(TABLE_SHARED_MEMORY, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd1 == -1) {
        perror("Cannot create table shared memory\n");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(fd1, sizeof(table)) == -1 ) {
        printf("Cannot initialize table segment size\n");
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
    sem_unlink("/OVEN");
    sem_unlink("/TABLE");
    shm_unlink(TABLE_SHARED_MEMORY);
    shm_unlink(OVEN_SHARED_MEMORY);
    system("make clean");
    exit(0);

}



int main() {

    signal(SIGINT, handle_sigint);

    set_semaphores();
    create_shared_memory();


    for(int i = 0; i < NUM_OF_COOKS; i++) {

        pid_t pid;
        if ((pid=fork()) == 0) {

            execl("./cook", "cook", NULL);
        }
        else if (pid == -1) {
            perror("cannot create child process while calling cook\n");
            exit(EXIT_FAILURE);
        }
        else {
            cooks_pids[i] = pid;

        }
        usleep(200000);
    }

    sleep(1);

    for(int j = 0; j < DELIVERYMEN; j++) {

        pid_t pid;
        if ((pid=fork()) == 0) {

            execl("./deliveryman", "deliveryman", NULL);
        }
        else if (pid == -1) {
            perror("cannot create child process while calling delivery man\n");
            exit(EXIT_FAILURE);
        }
        else {
            deliverymen_pids[j] = pid;
        }
        usleep(200000);
    }
    for (int i = 0; i < NUM_OF_COOKS;i++) {
        wait(NULL);
    }
    for (int i = 0; i < DELIVERYMEN;i++) {
        wait(NULL);
    }


    while(1) {}


    return 0;
}