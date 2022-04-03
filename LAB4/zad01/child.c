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


int main(int argc, char**argv) {

    char * status = argv[1];

    if (strcmp(status, "pending") != 0) {
        raise(SIGUSR1);
    }

    if (strcmp(status, "mask") == 0 || strcmp(status, "pending") == 0) {
        sigset_t newmask;
        sigpending(&newmask);
        printf("Exec signal pending: %d\n", sigismember(&newmask, SIGUSR1));
    }



}