//
// Created by tomasz on 06.05.22.
//


#include "shared.h"

int semaphore_id;
int pizza_type;
int oven_id;
int table_id;

void set_semaphores() {


}


void initialize_shared_memory() {


}


void place_in_oven() {



}

void place_on_table() {


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


    while(1) {
        place_in_oven();
        place_on_table();
    }

    return 0;
}