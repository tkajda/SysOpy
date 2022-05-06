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

    oven *ove = mmap(NULL,
                     sizeof(oven),
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     oven_memory,
                     0);

    if (ove ==  (void *) -1) {
        perror("Cannot mmap oven");
        exit(EXIT_FAILURE);
    }


}

void place_on_table() {




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