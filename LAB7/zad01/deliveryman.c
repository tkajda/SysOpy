//
// Created by tomasz on 05.05.22.
//
#include "shared.h"

int table_id;
int semaphore_id;

void set_semaphore() {

    key_t sem_key = ftok(PATHNAME, 'S');

    if ((semaphore_id = semget(sem_key, 0, 0))==-1) {
        perror("cannot get semaphore");
        exit(EXIT_FAILURE);
    }
}



void initialize_shared_memory() {

    //table block
    key_t table_key = ftok(PATHNAME, 'T');
    if (table_key == -1) {
        perror("Cannot generate key\n");
        exit(EXIT_FAILURE);
    }

    if ((table_id = shmget(table_key, 0,0)) == -1 ){
        perror("No permission to use shared memory\n");
        exit(EXIT_FAILURE);
    }

}

void get_and_deliver() {

    table *table = shmat(table_id,NULL, 0);

    //take first prepared

    int i = 0;
//    for(int j = 0; j < TABLE_SIZE; j++) {
//        printf("%d   ", table->pizzas[j]);
//    }
//    printf("\n");

    for(; i < TABLE_SIZE; i++) {
        if (table->is_taken[i] == 1) {
            table->is_taken[i] = 0;
            int k = 0;
            for( int j = 0; j < TABLE_SIZE; j++) {
                if (table->is_taken[j] == 1) k ++;
            }
            printf("(%d %ld) Pobieram pizze: %d. Liczba pizz na stole:%d\n",
                   getpid(), time(NULL), table->pizzas[i], k);
            break;
        }
    }
    sleep(4);
    printf("Dostarczam pizze: %d\n", table->pizzas[i]);

}




void handle_sigint() {
    semctl(semaphore_id, 0, IPC_RMID, NULL);
    shmctl(table_id, IPC_RMID, NULL);
    exit(0);
}

int main() {

    signal(SIGINT, handle_sigint);

    initialize_shared_memory();
    set_semaphore();
    while(1) {
        get_and_deliver();
    }


    return 0;
}