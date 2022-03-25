//
// Created by Tomasz Kajda on 23.03.2022.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

    if (argc!=2) exit(1);
    char * endptr;
    long n = strtol(argv[1], &endptr, 10);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if(pid==-1) {
            perror("fork failed");
            exit(1);
        }

        if (pid==0) {
            printf("child: %d, process pid: %d\n",i+1, getpid());
            return(i+1);
        }
        else {
            int j;
            pid = wait(&j);
        }

    }
    return 0;

}

