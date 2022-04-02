
#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

void ignore() {
    signal(SIGUSR1, SIG_IGN);
    while(1) {
        printf("Hello from PID: %d time: %ld\n", getpid(), clock());
        sleep(1);
    }
}




int main(int argc, char** argv) {

    printf("dzialanie dla: %s\n", argv[1]);

    if (strcmp(argv[1],"ignore")==0) {
        pid_t childpid = fork();

        if (childpid==0) {
            ignore();
        }
        else {
            kill(childpid, SIGUSR1);
            sleep(4);
            kill(childpid, SIGKILL);
        }


    }

    else if (strcmp(argv[1],"handle")==0) {}
    else if (strcmp(argv[1],"mask")==0) {}
    else if (strcmp(argv[1],"pending")==0) {}
    else {
            return 1;
    }



    return 0;
} 