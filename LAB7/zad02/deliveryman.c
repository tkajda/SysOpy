//
// Created by tomasz on 06.05.22.
//

#include "shared.h"

int table_id;
int semaphore_id;
sem_t *table_semaphore;
int pizza_type;
int table_memory;


void set_semaphore() {
    table_semaphore = sem_open(TABLE_SEMAPHORE, 0);
    if (table_semaphore == SEM_FAILED) {
        perror("Cannot open table semaphore");
        exit(EXIT_FAILURE);
    }
}



void initialize_shared_memory() {

    table_memory = shm_open(TABLE_SHARED_MEMORY, O_RDWR, 0666);
    if (table_memory == -1) {
        printf("Cannot access shared memory\n");
        exit(EXIT_FAILURE);
    }
}


void get_and_deliver() {
    table *table = mmap(NULL,
                        sizeof(table),
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED,
                        table_memory,
                        0);

    if (table ==  (void *) -1) {
        perror("Cannot mmap table");
        exit(EXIT_FAILURE);
    }
    //take first prepared

    int pizza_type;
    int flag = 0;
    while(flag<1) {
        for(int i = 0; i < TABLE_SIZE; i++) {
            if (table->is_taken[i] == 1) {

                table->is_taken[i] = 0;
                pizza_type =  table->pizzas[i];

                //count pizzas left on the table
                int k = 0;
                for( int j = 0; j < TABLE_SIZE; j++) {
                    if (table->is_taken[j] == 1) k ++;
                }
                printf("(%d %ld) Pobieram pizze: %d. Liczba pizz na stole:%d\n",
                       getpid(), time(NULL),pizza_type, k);
                flag = 1;
                break;
            }
        }
    }


    if(sem_wait(table_semaphore) == -1){
        perror("Cannot wait for table in deliveryman");
        exit(EXIT_FAILURE);
    }


    if(sem_post(table_semaphore) == -1){
        perror("Cannot post for table in cook");
        exit(EXIT_FAILURE);
    }

    rand_sleep(4,5);
    printf("(%d) Dostarczam pizze: %d\n",getpid(), pizza_type);

    munmap(table, sizeof(table));

}




void handle_sigint() {
    sem_close(table_semaphore);
    shm_unlink(TABLE_SHARED_MEMORY);
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