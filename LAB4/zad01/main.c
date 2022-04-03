
#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

void handle_sigusr1(int sig) {
    printf("signal handled\n");
}


void funcForChild(char* status) {



}




int main(int argc, char** argv) {

    printf("%s\n", argv[1]);
    pid_t childpid = fork();
    char * status = argv[1];


    if (childpid==0) {
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


        while(1) {
            printf("Hello from PID: %d time: %ld\n", getpid(), clock());
            usleep(500000);
        }

    }

    sleep(1);
    kill(childpid, SIGUSR1);

    sleep(2);
    kill(childpid, SIGKILL);
    sleep(1.5);




    return 0;
} 