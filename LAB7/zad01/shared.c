//
// Created by tomasz on 05.05.22.
//
#include "shared.h"

int semaphore_id;
int oven_id;
int table_id;

void set_semaphores() {
    key_t sem_key = ftok(PATHNAME, 'S');

    if ((semaphore_id = semget(sem_key, 2, IPC_CREAT | 0666))==-1) {
        perror("cannot create semaphore");
        exit(EXIT_FAILURE);
    }

    //oven semaphore
    if (semctl(semaphore_id, 1, GETVAL, NULL) == -1) {
        perror("cannot initialize semaphore 1");
        exit(EXIT_FAILURE);
    }

    //table semaphore
    if (semctl(semaphore_id, 0, GETVAL, NULL) == -1) {
        perror("cannot initialize semaphore 2");
        exit(EXIT_FAILURE);
    }

}


void create_shared_memory() {
    //table semaphore
    key_t table_key = ftok(PATHNAME, 'T');
    if (table_key == -1) {
        perror("Cannot generate key\n");
        exit(EXIT_FAILURE);
    }

    table_id;
    if ((table_id = shmget(table_key, sizeof(table), IPC_CREAT | 0666)) == -1 ){
        perror("Cannot create shared memory\n");
        exit(EXIT_FAILURE);
    }
    shmat(table_id, NULL, 0);

    //oven semaphore
    key_t oven_key = ftok(PATHNAME, 'O');
    if (oven_key == -1) {
        perror("Cannot generate key\n");
        exit(EXIT_FAILURE);
    }

    oven_id;
    if ((oven_id = shmget(oven_key, sizeof(oven), IPC_CREAT | 0666)) == -1 ){
        perror("No permission to use the semaphore\n");
        exit(EXIT_FAILURE);
    }
    shmat(oven_id, NULL, 0);


}




int main() {


    set_semaphores();
    create_shared_memory();



    printf("OVEN KEY MAIN %d\n", oven_id);



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
        sleep(1);
    }
    for (int i = 0; i < NUM_OF_COOKS;i++) {
        wait(NULL);
    }


//    for(int j = 0; j < DELIVERYMEN; j++) {
//
//        pid_t pid;
//        if ((pid==fork()) == 0) {
//
//            execl("./deliveryman", "deliveryman", NULL);
//        }
//        else if (pid == -1) {
//            perror("cannot create child process while calling delivery man\n");
//            exit(EXIT_FAILURE);
//        }
//        else {
//            wait(NULL);
//        }
//    }
//    for (int i = 0; i < DELIVERYMEN;i++) {
//        wait(NULL);
//    }


    sleep(15);

    return 0;
}