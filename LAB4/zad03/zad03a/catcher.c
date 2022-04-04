//
// Created by Tomasz Kajda on 04.04.2022.
//

#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>


void handler(int sig, siginfo_t *info, void *ucontext) {
    printf("HANDLED\n");
    static int counter = 0;

    if(sig==SIGUSR1) {
        counter++;
    }
    int senderPID = info->si_pid;

    printf("%d", senderPID);

}



int main(int argc, char** argv) {

    char str[12];
    sprintf(str,"%d",getpid());

    if (!(argc<3 || strcmp(argv[2], "kill") != 0 || strcmp(argv[2], "SIGRT") != 0 ||
            strcmp(argv[2], "SIGQUEUE") != 0 ) ){
        perror("not able to cope");
        exit(EXIT_FAILURE);
    }
    printf("\ncatcher with pid: %d\n", getpid());

    pid_t childpid = fork();
    if (childpid==0) {
        execl("./sender", "./sender",argv[1],str,argv[2], NULL);
    }
    else {
        struct sigaction sa;
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = handler;
        sigaction(SIGUSR1, &sa, NULL);
        while(1) {
            sleep(1);
        }
    }

//    sigaction(SIGUSR1, &act2, NULL);
    return 0;

}