//
// Created by tomasz on 05.05.22.
//


#include "shared.h"

int semaphore_id;
int pizza_type;
int oven_id;
int table_id;

void set_semaphores() {

    //oven semaphore
    key_t sem_key = ftok(PATHNAME, 'S');

    if ((semaphore_id = semget(sem_key, 0, 0))==-1) {
        perror("cannot get semaphore");
        exit(EXIT_FAILURE);
    }


}


void initialize_shared_memory() {
    //oven block
    key_t oven_key = ftok(PATHNAME, 'O');
    if (oven_key == -1) {
        perror("Cannot generate key\n");
        exit(EXIT_FAILURE);
    }

    if ((oven_id = shmget(oven_key, 0,0)) == -1 ){
        perror("No permission to use shared memory\n");
        exit(EXIT_FAILURE);
    }

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


void place_in_oven() {

    srand(time(NULL)  ^ (getpid()<<16) );

    pizza_type = rand() % 10;

    sleep(1);
    printf("(%d %ld) Przygotowuje pizze: %d\n", getpid(), time(NULL), pizza_type);

    oven *oven = shmat(oven_id, NULL, 0);

    sembuf *load_oven = calloc(1, sizeof(sembuf));
    load_oven->sem_op = -1;
    load_oven->sem_flg = SEM_UNDO;

    semop(semaphore_id, load_oven, 0);

    oven->pizzas[oven->last_taken%OVEN_SIZE] = pizza_type;
    oven->last_taken ++;

    shmdt(oven);

}

void place_on_table() {
    table *table = shmat(table_id,NULL, 0);

    printf("%d placing on table %d\n",getpid(),  table->last_taken%TABLE_SIZE);
    sembuf *load_table = calloc(1, sizeof(sembuf));

    semop(semaphore_id, load_table, 1);

    table->pizzas[table->last_taken%TABLE_SIZE] = pizza_type;
    table->is_taken[table->last_taken%TABLE_SIZE] = 1;
    table->last_taken ++;

    shmdt(table);

}

void handle_sigint() {
    semctl(semaphore_id, 0, IPC_RMID, NULL);
    shmctl(oven_id, IPC_RMID, NULL);
    shmctl(table_id, IPC_RMID, NULL);
    exit(0);
}




int main() {

    signal(SIGINT, handle_sigint);
    initialize_shared_memory();
    set_semaphores();


    int iterations = 0;

    while(iterations<3) {
        place_in_oven();
        sleep(4);
        place_on_table();
    }

    return 0;
}