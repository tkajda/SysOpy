//
// Created by Tomasz Kajda on 03.04.2022.
//

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char**argv) {

    char * status = argv[1];

    if (strcmp(status, "pending") != 0) {
        raise(SIGUSR1);
    }

    if (strcmp(status, "mask") == 0 || strcmp(status, "pending") == 0) {

        sigset_t pending_signals;
        sigpending(&pending_signals);
        if (sigismember(&pending_signals, SIGUSR1)) {
            printf("signal is pending\n");

        } else {
            printf("signal is not pending\n");
        }

        usleep(50000);
    }
    return 0;

}
