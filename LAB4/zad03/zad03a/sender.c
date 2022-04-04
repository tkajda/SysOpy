//
// Created by Tomasz Kajda on 04.04.2022.
//

#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


void handle(int sig, siginfo_t *info, void *ucontext) {
    printf("SIEMA");
}


int main(int argc, char** argv) {

//    sleep(5);
//    exit(0);
    if (argc < 3) {
        exit(1);
    }

    int catcherPID = atoi(argv[2]);
    int n = atoi(argv[1]);
//    char* mode = argv[3];

    struct sigaction act2;
    act2.sa_flags = SA_SIGINFO;
    act2.sa_sigaction = handle;
    sigemptyset(&act2.sa_mask);

    int i = 0;
    while (i < n) {
        sleep(1);
        kill(catcherPID, SIGUSR1);
        i++;
    }
    exit(0);

}