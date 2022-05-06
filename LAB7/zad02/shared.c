//
// Created by tomasz on 06.05.22.
//
#include "shared.h"

int semaphore_id;
int oven_id;
int table_id;
pid_t cooks_pids[NUM_OF_COOKS];
pid_t deliverymen_pids[DELIVERYMEN];



void set_semaphores() {



}


void create_shared_memory() {


}



void handle_sigint() {
    for (int i = 0; i < NUM_OF_COOKS; i++){
        kill(cooks_pids[i], SIGINT);
    }
    for (int j = 0; j < DELIVERYMEN; j++){
        kill(deliverymen_pids[j], SIGINT);
    }


    system("make clean");
    exit(0);

}



int main() {

    signal(SIGINT, handle_sigint);

    set_semaphores();
    create_shared_memory();

    for(int i = 0; i < NUM_OF_COOKS; i++) {

        pid_t pid;
        cooks_pids[i] = pid;

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
        usleep(200000);
    }
    for (int i = 0; i < NUM_OF_COOKS;i++) {
        wait(NULL);
    }
    sleep(3);


    for(int j = 0; j < DELIVERYMEN; j++) {

        pid_t pid;
        deliverymen_pids[j] = pid;
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
        sleep(1);
    }
    for (int i = 0; i < DELIVERYMEN;i++) {
        wait(NULL);
    }


    while(1) {}


    return 0;
}