//
// Created by Tomasz Kajda on 03.04.2022.
//

#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

void handle_sigusr1(int sig) {
    printf("signal handled in proccess %d\n", getpid());
}


void funcForChild() {
    while(1) {
        printf("Hello from PID: %d time: %ld\n", getpid(), clock());
        usleep(500000);
    }


}


int main(int argc, char** argv) {

    printf("%s\n", argv[1]);
    printf("main PID = %d\n", getpid());
    char * status = argv[1];


    if (strcmp(status,"ignore")==0) {
        signal(SIGUSR1, SIG_IGN);
    }
    else if (strcmp(status,"handle")==0) {
        struct sigaction sa;
        sa.sa_handler = &handle_sigusr1;
        sigaction(SIGUSR1, &sa, NULL);
    }
    else if (strcmp(status,"mask")==0) {
        sigset_t newmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGUSR1);
        if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0)
            perror("cannot block signal\n");
        if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0) {
            perror("signal blocked\n");
        }

    }
    else if (strcmp(status,"pending")==0) {
    }
    else {
        exit(1);
    }

    pid_t childpid = fork();

    if (childpid==0) {
        funcForChild();
    }

    sleep(1);
    kill(childpid, SIGUSR1);

    sleep(2);
    kill(childpid, SIGKILL);




    return 0;
} 