//
// Created by Tomasz Kajda on 03.04.2022.
//
#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


void handle(int sig, siginfo_t *info, void *ucontext) {
    printf("sig: %d\n"
           "Signal number: %d\n"
           "PID: %d\n"
           "Real user ID: %d\n"
           "Signal code: %d\n\n",
           sig,
           info->si_signo,
           info->si_pid,
           info->si_uid,
           info->si_code);
}




int main(int argc, char ** argv) {

    struct sigaction act;
    sigemptyset(&act.sa_mask);

    act.sa_flags = SA_SIGINFO;
    printf("SA_SIGINFO TEST\nhandle SIGUSR1\n");
    act.sa_sigaction = handle;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1);

    printf("SA_SIGINFO TEST\nhandle SIGUSR2\n");
    sigaction(SIGUSR2, &act, NULL);
    raise(SIGUSR2);


    printf("SA_SIGINFO TEST\nhandle SIGPIPE\n");
    sigaction(SIGPIPE, &act, NULL);
    raise(SIGPIPE);



    printf("SA_NOCLDSTOP TEST\nhandle SIGCHLD\n");

    struct sigaction act2;
    act2.sa_flags = SA_SIGINFO | SA_NOCLDSTOP;
    act2.sa_sigaction = handle;
    sigemptyset(&act2.sa_mask);
    sigaction(SIGCHLD, &act2, NULL);


    pid_t childpid = fork();

    if (childpid == 0) {
        raise(SIGCHLD);
    }


    printf("SA_ONSTACK TEST\nhandle SIGCONT\n");
    struct sigaction act3;
    act3.sa_flags = SA_SIGINFO | SA_ONSTACK;
    act3.sa_sigaction = handle;
    sigemptyset(&act3.sa_mask);
    sigaction(SIGCONT, &act, NULL);
    raise(SIGCONT);

    return 0;

}