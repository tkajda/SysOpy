//
// Created by tomasz on 29.04.22.
//
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "stdbool.h"

#include "header.h"

#define MAX_COMMAND_LEN 256

int server_qid;
int client_qid;
int id;

void stop_chatting() {
    raise(SIGINT);
}



int open_server_queue() {
    key_t key = ftok(getenv("HOME"), 1);
    if (key == -1) {
        perror("Cannot generate server key");
        exit(EXIT_FAILURE);
    }
    return msgget(key, 0);
}



int main() {


    if ((server_qid = open_server_queue()) == -1) {
        perror("Cannot connect to server");
        exit(EXIT_FAILURE);
    }
    printf("Welcome %d on server: %d\n",getpid(), server_qid);




    return 0;
}