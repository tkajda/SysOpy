
#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

void handle_sigusr1(int sig) {

}


void funcForChild(char* status) {

    if (strcmp(status,"ignore")==0) {
        signal(SIGUSR1, SIG_IGN);
    }
    else if (strcmp(status,"handle")==0) {
        struct sigaction sa;
        sa.sa_handler = &handle_sigusr1;
        sigaction(SIGUSR1, &sa, NULL);

    }
    else if (strcmp(status,"mask")==0) {}
    else if (strcmp(status,"pending")==0) {}
    else {
        exit(0);
    }

    while(1) {
        printf("Hello from PID: %d time: %ld\n", getpid(), clock());
        sleep(1);
    }

}




int main(int argc, char** argv) {

    printf("%s\n", argv[1]);
    pid_t childpid = fork();

    if (childpid==0) {
        funcForChild(argv[1]);
    }
    else {
        printf("waiting...");
        sleep(2);
        printf("signal sent");
        kill(childpid, SIGUSR1);
        printf("waiting...");

        sleep(4);
        kill(childpid, SIGKILL);
        sleep(3);
        printf("waiting...");


    }


    return 0;
} 