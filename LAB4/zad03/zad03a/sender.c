//
// Created by Tomasz Kajda on 04.04.2022.
//

#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

static int noSignals = 0;
char * mode;
static sigset_t mask;


void handle(int sig, siginfo_t *info, void *ucontext) {

    if (sig == SIGUSR1) {
        noSignals++;
        sigsuspend(&mask);
    }
    else if (sig == SIGUSR2 && strcmp(mode,"kill")!=0) {
        printf("sender: received %d SIGUSR1 signals should have received %d\n", noSignals, info->si_value.sival_int);
    }
    else if (sig == SIGUSR2) {
        printf("Sender received %d signals\n",  noSignals);
        exit(0);
    }
    if (strcmp(mode,"sigqueue") == 0) {
        printf("Sender sent %d signals\n",  info->si_value.sival_int);
    }
}


int main(int argc, char** argv) {


    if (argc < 3) {
        exit(1);
    }

    printf("sender pid: %d\n", getpid());


    int catcherPID = atoi(argv[2]);
    int n = atoi(argv[1]);
    mode = argv[3];

    sigfillset(&mask);
    sigdelset(&mask,SIGUSR1);
    sigdelset(&mask,SIGUSR2);


    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handle;


    if (sigprocmask(SIG_BLOCK,&mask,NULL) < 0) {
        exit(EXIT_FAILURE);
    }
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    if (strcmp(mode, "kill") == 0) {
        int i = 0;
        while (i < n) {
            kill(catcherPID, SIGUSR1);
            i++;
        }
        kill(catcherPID,SIGUSR2);

        sigsuspend(&mask);
    }

    else if(strcmp(mode,"sigqueue") == 0) {

        union sigval value1;
        value1.sival_ptr = NULL;
        for(int i = 0; i < n; i++) {
            sigqueue(catcherPID, SIGUSR1, value1);
            sigsuspend(&mask);
        }
        sigqueue(catcherPID, SIGUSR2, value1);

    }

    exit(0);

}