//
// Created by tomasz on 06.05.22.
//

#include "shared.h"

int table_id;
int semaphore_id;

void set_semaphore() {

}



void initialize_shared_memory() {

}


void get_and_deliver() {


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