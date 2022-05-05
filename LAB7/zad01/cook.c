//
// Created by tomasz on 05.05.22.
//


#include "shared.h"

int semaphore_id;
int pizza_type;
int oven_id;
int table_id;

void set_semaphores() {
    key_t sem_key = ftok(PATHNAME, 'S');

    if ((semaphore_id = semget(sem_key, 0, IPC_CREAT | 0666))==-1) {
        perror("cannot get semaphore");
        exit(EXIT_FAILURE);
    }

    if (semctl(semaphore_id, 1, GETVAL, NULL) == -1) {
        perror("cannot initialize semaphore 1");
        exit(EXIT_FAILURE);
    }



}

void initialize_shared_memory() {
    key_t oven_key = ftok(PATHNAME, 'O');
    if (oven_key == -1) {
        perror("Cannot generate key\n");
        exit(EXIT_FAILURE);
    }

    oven_id;
    if ((oven_id = shmget(oven_key, 0,0)) == -1 ){
        perror("No permission to use shared memory\n");
        exit(EXIT_FAILURE);
    }

    key_t table_key = ftok(PATHNAME, 'T');
    if (table_key == -1) {
        perror("Cannot generate key\n");
        exit(EXIT_FAILURE);
    }

    table_id;
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

    semop(semaphore_id, load_oven, 1);

    oven->capacity[oven->last_taken%OVEN_SIZE] = pizza_type;
    oven->last_taken ++;

}

void place_on_table() {
    table *table = shmat(table_id,NULL, 0);

    printf("%d placing on table %d\n",getpid(),  table->last_taken%TABLE_SIZE);
    sembuf *load_table = calloc(2, sizeof(sembuf));

    semop(semaphore_id, load_table, 1);

    table->capacity[table->last_taken%TABLE_SIZE] = pizza_type;
    table->last_taken ++;

}


int main() {

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