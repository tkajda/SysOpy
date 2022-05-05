//
// Created by tomasz on 05.05.22.
//
#include "shared.h"


int main() {

    int *table = calloc(TABLE_SIZE, sizeof(int));
    int *oven = calloc(OVEN_SIZE, sizeof(int));


    //table semaphore
    key_t table_key = ftok(getenv("HOME"), 'Z');
    if (table_key == -1) {
        perror("Cannot generate key\n");
        exit(EXIT_FAILURE);
    }

    int shared_mem_id;
    if ((shared_mem_id = shmget(table_key, 10000, IPC_CREAT | 0666)) == -1 ){
        perror("Cannot create shared memory\n");
        exit(EXIT_FAILURE);
    }


    //oven semaphore
    key_t oven_key = ftok(getenv("HOME"), 'A');
    if (oven_key == -1) {
        perror("Cannot generate key\n");
        exit(EXIT_FAILURE);
    }

    int shared_mem_id_1;
    if ((shared_mem_id_1 = shmget(oven_key, 10000, IPC_CREAT | 0666)) == -1 ){
        perror("Cannot create shared memory\n");
        exit(EXIT_FAILURE);
    }



    for(int i = 0; i < NUM_OF_COOKS; i++) {

        pid_t pid;
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
    }
    for (int i = 0; i < NUM_OF_COOKS;i++) {
        wait(NULL);
    }


    for(int j = 0; j < DELIVERYMEN; j++) {

        pid_t pid;
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
    }
    for (int i = 0; i < DELIVERYMEN;i++) {
        wait(NULL);
    }




    free(table);
    free(oven);

    return 0;
}