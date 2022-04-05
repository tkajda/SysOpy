//
// Created by Tomasz Kajda on 04.04.2022.
//

#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


static int counter = 0;

char* mode;
static sigset_t mask;

void sendinfo(pid_t senderPID, int sig) {
    union sigval value;
    value.sival_int = counter;
    value.sival_ptr = NULL;
    sigqueue(senderPID, sig, value);
}


void handler(int sig, siginfo_t *info, void *ucontext) {

    int senderPID = info->si_pid;

    if(sig==SIGUSR1 && strcmp(mode,"sigqueue")!=0) {
        counter++;
        sigsuspend(&mask);
    }
    else if(sig==SIGUSR2 && strcmp(mode,"kill") == 0) {
        int i = 0;

        while(i++<counter) {
            kill(senderPID,SIGUSR1);
            kill(senderPID,SIGUSR1);
        }
        kill(senderPID,SIGUSR2);
        printf("catcher: received %d signals\n", counter);
        kill(senderPID, SIGKILL);
        exit(0);
    }
    else if (strcmp(mode,"sigqueue")==0 && sig==SIGUSR2) {
        sendinfo(senderPID, SIGUSR2);
        printf("catcher: received %d signals\n", counter);
        exit(0);
    }
    else if (strcmp(mode,"sigqueue")==0 && sig == SIGUSR1) {
        counter++;
        sendinfo(senderPID, SIGUSR1);
        sigsuspend(&mask);
    }

}



int main(int argc, char** argv) {

    char str[12];
    sprintf(str, "%d", getpid());

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;

    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGUSR2);


    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    mode = argv[2];

    if (!(argc < 3 || strcmp(argv[2], "kill") != 0 || strcmp(argv[2], "SIGRT") != 0 ||
          strcmp(argv[2], "sigqueue") != 0)) {
        perror("not able to cope");
        exit(EXIT_FAILURE);
    }


    printf("\ncatcher, pid: %d\n", getpid());
    pid_t childpid = fork();

    if (childpid == 0) {
        execl("./sender", "./sender", argv[1], str, argv[2], NULL);

    } else if (childpid < 0) {
        printf("failed to fork");
        exit(1);
    } else {
        if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
            exit(EXIT_FAILURE);
        }
        sigsuspend(&mask);
    }
    return 0;

}