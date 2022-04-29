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
key_t my_key;
int client_qid;
int id;

void handle_sigint() {
    printf("SIGNAL HANDLED\n");
}




void stop() {
    raise(SIGINT);
}

void send_to_server(msg_t *msg) {
    msg->timestamp = time(NULL);
    if(msgsnd(server_qid, msg, MAX_MESSAGE_SIZE, 0) == -1) {
        perror("Cannot send message\n");
    }
}

int create_user_queue() {

    my_key = ftok("HOME", getpid() % 'D');

    if ((client_qid = msgget(my_key, IPC_CREAT | IPC_EXCL | 0777))==-1) {
        perror("Cannot open clients queue\n");
    }

    msg_t msg;
    msg.type = INIT;
    sprintf(msg.text, "%d", my_key);
    msg.pid = getpid();
    send_to_server(&msg);

    if (msgrcv(client_qid, &msg,MAX_MESSAGE_SIZE,-100,0) == -1) {
        perror("Cannot receive id from server");
        exit(EXIT_FAILURE);
    }

    printf("You are client number %d\n", msg.id);

}



void delete_queue() {
    msg_t msg;
    msg.type = STOP;
    msg.id = id;
    sprintf(msg.text, " ");
    printf("Client disconnected\n");

    send_to_server(&msg);


    msgctl(client_qid, IPC_RMID, NULL);


}





int open_server_queue() {
    key_t key = ftok(getenv("HOME"), 1);
    if (key == -1) {
        perror("Cannot generate server key");
        exit(EXIT_FAILURE);
    }
    return msgget(key, 0);
}


void handle_shutdown() {
    printf("Server shutdown\n");
    exit(EXIT_SUCCESS);
}


void init()
{
    atexit(delete_queue);
    signal(SIGINT, handle_sigint);

    if ((server_qid = open_server_queue()) == -1) {
        perror("Cannot connect to server");
        exit(EXIT_FAILURE);
    }

    create_user_queue();
}


int is_empty(int qid) {

    struct msqid_ds buf;
    msgctl(qid, IPC_STAT, &buf);

    return buf.msg_qnum == 0;
}

void handle_2all(msg_t *msg) {
    printf("handled 2all\n");

}

void handle_2one(msg_t *msg) {
    printf("handled 2one\n");
}



void handle_msg() {
    while(!is_empty(client_qid))  {
        msg_t msg;
        if(msgrcv(client_qid, &msg, MAX_MESSAGE_SIZE, -100, IPC_NOWAIT) != -1) {
            switch(msg.type) {
                case TOALL:
                    handle_2all(&msg);
                    break;
                case TOONE:
                    handle_2one(&msg);
                    break;
                case SHUTDOWN:
                    handle_shutdown();
                    break;
            }
        }
    }
}




int main() {

    init();

    while(1) {
        handle_msg();


    }


    printf("Welcome %d on server: %d\n",getpid(), server_qid);




    return 0;
}