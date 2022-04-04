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

static int counter = 0;

char* mode;


void handler(int sig, siginfo_t *info, void *ucontext) {

    printf("%d\n", sig);
    if(sig==SIGUSR1) {
        counter++;
    }
    else if(sig==SIGUSR2) {
        printf("received %d signals\n", counter);
        exit(0);
    }
    int senderPID = info->si_pid;

    if (strcmp(mode,"kill")==0) {
        kill(senderPID, SIGUSR2);
    }
    else if (strcmp(mode,"sigrt")==0) {
        int rSignal = SIGRTMIN + 1;
//        int rSingal2 = SIGRTMIN +2;
        kill(senderPID, rSignal);
    }
    else if (strcmp(mode,"sigqueue")==0) {
        union sigval value;
        value.sival_ptr = NULL;
        for(int sig_ctr = 0; sig_ctr < senderPID; sig_ctr++)
            sigqueue(info->si_pid, SIGUSR1, value);
        sigqueue(info->si_pid, SIGUSR2, value);
    }


}



int main(int argc, char** argv) {

    char str[12];
    sprintf(str,"%d",getpid());
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask,SIGUSR1);
    sigdelset(&mask,SIGUSR2);

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    mode = argv[2];

    if (!(argc<3 || strcmp(argv[2], "kill") != 0 || strcmp(argv[2], "SIGRT") != 0 ||
            strcmp(argv[2], "SIGQUEUE") != 0 ) ){
        perror("not able to cope");
        exit(EXIT_FAILURE);
    }


    printf("\ncatcher, pid: %d\n", getpid());

    pid_t childpid = fork();
    if (childpid==0) {
        execl("./sender", "./sender",argv[1],str,argv[2], NULL);
        exit(0);
    }
    else if (childpid < 0) {
        printf("failed to fork");
        exit(1);
    }
    else {
        if (sigprocmask(SIG_BLOCK,&mask,NULL) < 0) {
            exit(EXIT_FAILURE);
        }
        for (int i = 0 ; i < atoi(argv[1])/2 ;i++) {
            usleep(500000);
        }
//        sleep(2);
//        printf("%d\n", counter);
    }
    //    sigaction(SIGUSR1, &act2, NULL);
    return 0;

}