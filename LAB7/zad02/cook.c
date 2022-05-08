//
// Created by tomasz on 06.05.22.
//


#include "shared.h"

sem_t *table_semaphore;
sem_t *oven_semaphore;
int pizza_type;
int oven_memory;
int table_memory;


void set_semaphores() {
    table_semaphore = sem_open("/TABLE", O_RDWR);
    if (table_semaphore == SEM_FAILED) {
        perror("Cannot open table semaphore");
        exit(EXIT_FAILURE);
    }

    oven_semaphore = sem_open("/OVEN", O_RDWR);
    if (oven_semaphore == SEM_FAILED) {
        perror("Cannot open oven semaphore");
        exit(EXIT_FAILURE);
    }

}


void initialize_shared_memory() {

    oven_memory = shm_open(OVEN_SHARED_MEMORY, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    if (oven_memory == -1) {
        printf("Cannot access shared memory\n");
        exit(EXIT_FAILURE);
    }

    table_memory = shm_open(TABLE_SHARED_MEMORY, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    if (table_memory == -1) {
        printf("Cannot access shared memory\n");
        exit(EXIT_FAILURE);
    }


}


void place_in_oven() {
    oven *oven = mmap(NULL,
                     sizeof(oven),
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     oven_memory,
                     0);

    if (oven ==  (void *) -1) {
        perror("Cannot mmap oven");
        exit(EXIT_FAILURE);
    }
    pizza_type = rand() % 10;
    printf("(%d %ld) Przygotowuje pizze: %d\n", getpid(), time(NULL), pizza_type);
    sleep(1, 2);

    if(sem_wait(oven_semaphore) == -1){
        perror("Cannot wait for oven in cook");
        exit(EXIT_FAILURE);
    }

    int flag = 0;
    while(flag < 1) {
        for (int i = 0; i < OVEN_SIZE; i++) {
            if (oven->is_taken[oven->last_taken%OVEN_SIZE] == 0) {

                oven->is_taken[oven->last_taken%OVEN_SIZE] = 1;
                oven->pizzas[oven->last_taken%OVEN_SIZE] = pizza_type;

                oven->is_taken[oven->last_taken%OVEN_SIZE] = 0;
                oven->last_taken ++;
                flag = 1;
                break;
            }
        }
    }

    if(sem_post(oven_semaphore) == -1){
        perror("Cannot post for oven in cook");
        exit(EXIT_FAILURE);
    }

    rand_sleep(4, 5);

    munmap(oven, sizeof(oven));
}


void place_on_table() {
    table *table = mmap(NULL,
                      sizeof(table),
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED,
                        table_memory,
                      0);

    if (table ==  (void *) -1) {
        perror("Cannot mmap oven");
        exit(EXIT_FAILURE);
    }
    printf("%d placing on table %d\n",getpid(),  table->last_taken%TABLE_SIZE);

    if(sem_wait(table_semaphore) == -1){
        perror("Cannot wait for table in cook");
        exit(EXIT_FAILURE);
    }

    int flag = 0;
    while(flag<1) {
        for(int i = 0 ; i < TABLE_SIZE; i++) {
            if (table->is_taken[table->last_taken%TABLE_SIZE] == 0) {

                table->is_taken[table->last_taken%TABLE_SIZE] = 1;
                table->pizzas[table->last_taken%TABLE_SIZE] = pizza_type;
                table->last_taken ++;
                flag = 1;

                break;

            }
        }
    }
    if(sem_post(table_semaphore) == -1){
        perror("Cannot post for table in cook");
        exit(EXIT_FAILURE);
    }

    munmap(table, sizeof(table));
}


void handle_sigint() {
    exit(0);
}




int main() {

    signal(SIGINT, handle_sigint);
    initialize_shared_memory();
    set_semaphores();


    while(1) {
        place_in_oven();
        place_on_table();
    }

    return 0;
}