//
// Created by Tomasz Kajda on 03.04.2022.
//

#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void handle_sigusr1(int sig) {
    usleep(50000);
    fflush(NULL);
    printf("signal handled in proccess %d\n", getpid());
}

void funcForChild() {
    printf("Hello from PID: %d\n", getpid());
}


int main(int argc, char** argv) {

    char * status = argv[1];

    if (strcmp(status,"ignore")==0) {
        signal(SIGUSR1, SIG_IGN);
    }
    else if (strcmp(status,"handle")==0) {
        struct sigaction sa;
        sa.sa_handler = &handle_sigusr1;
        sigaction(SIGUSR1, &sa, NULL);
    }

    else if (strcmp(status,"mask")==0 || strcmp(status,"pending")==0) {
        sigset_t newmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGUSR1);
        if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0) {
            printf("unable to block signal");
        }
    }

    printf("main PID = %d\n", getpid());

    raise(SIGUSR1);
    sigset_t newmask;

    if (strcmp(status, "mask") == 0 || strcmp(status, "pending") == 0) {
        sigpending(&newmask);
        printf("Signal pending: %d\n", sigismember(&newmask, SIGUSR1));
    }
    if (strcmp(argv[2], "fork")==0){

        pid_t childpid = fork();

        if (childpid==0) {
            funcForChild();
        }

        if (strcmp(status, "pending") != 0) {
            raise(SIGUSR1);
        }

        if (strcmp(status, "mask") == 0 || strcmp(status, "pending") == 0) {
            sigset_t childmask;
            sigpending(&childmask);
            printf("Child signal pending: %d\n", sigismember(&childmask, SIGUSR1));
        }

    }
    else {
        execl("./child", "./child", status, NULL);
    }
    sleep(1);
    printf("\n\n");
    return 0;
} 