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
#include "stdbool.h"

#include "header.h"

#define MAX_NO_CLIENTS 128
#define BUFSIZE 128
#define INACTIVE -1

typedef struct client_t {
    int qid;
} client_t;

client_t clients[MAX_NO_CLIENTS];
FILE* file;
const char* filename = "server_log.txt";
int server_qid;
int clients_number = 0;


int open_server_queue() {
    key_t key = ftok(getenv("HOME"), 1);
    if (key == -1) {
        perror("Cannot generate server key");
        exit(EXIT_FAILURE);
    }
    int msget =  msgget(key, IPC_CREAT | IPC_EXCL | 0777);


    if (msget == -1) {
        perror("Cannot open server's queue");
        exit(EXIT_FAILURE);
    }
    printf("Server opened with id: %d\n", msget);
    return msget;
}

void send_to_client(int client_id, msg_t *msg) {
    if (client_id >= MAX_NO_CLIENTS || clients[client_id].qid == INACTIVE) {
        fprintf(stderr, "Client %d does not exist\n", client_id);
        return;
    }
    if (msgsnd(clients[client_id].qid, msg,MAX_MESSAGE_SIZE, 0)== -1 && clients[client_id].qid != INACTIVE) {
        fprintf(stderr, "Cannot send message to %d\n", client_id);
    }
}


void handler_init(msg_t *msg) {

    key_t client_key = atoi(msg->text);

    int client_id = -1;

    for (int i = 0; i < MAX_NO_CLIENTS; ++i) {
        if (clients[i].qid == INACTIVE) {
            clients_number++;
            client_id = i;
            clients[i].qid = client_id;
            break;
        }
        if (i == MAX_NO_CLIENTS) {
            perror("Cannot connect another client");
            exit(EXIT_FAILURE);
        }
    }

    printf("client id: %d\n", client_id);

    if(client_id != -1) {
        if ((clients[client_id].qid = msgget(client_key, 0)) == -1) {
            perror("Server unable to establish client id");
        }

        msg_t new_client_msg;
        new_client_msg.type = INIT;
        new_client_msg.id = client_id;
        send_to_client(client_id, &new_client_msg);
    }
    else {
        fprintf(stderr, "Unable to register another client\n");
    }
}

void update_log(msg_t *msg) {

    time_t t = msg->timestamp;

    char log[MAX_BUFF_SIZE];
    sprintf(log, "TYPE %2ld   TIME: %16s\n"
                 "TEXT: %5s\n"
                 "sender_id: %2d\n\n\n\n",
                 msg->type, asctime(localtime(&t)),
                 msg->text,
                 msg->id);

        fprintf(file, "%s", log);

}


void handle_sigint() {
    msg_t msg;
    msg.type = SHUTDOWN;
    for(int i = 0; i < MAX_NO_CLIENTS; ++i) {
        if (clients[i].qid != INACTIVE) {
            send_to_client(i, &msg);
        }
    }

    while(clients_number > 0) {
        if(msgrcv(server_qid, &msg, MAX_MESSAGE_SIZE, -100, 0) == -1) {
            perror("Server unable to receive message");
            continue;
        }
        if(msg.type == STOP) {
            update_log(&msg);
            clients_number--;
        }
    }

    printf("Server disconnected!\n");
    exit(EXIT_SUCCESS);
}


void handler_stop(msg_t *msg) {
    int client_id = msg->id;
    if(clients[client_id].qid == -1) {
        perror("Client inactive");
    }

    clients[client_id].qid = INACTIVE;
    clients_number--;
    printf("Client %d disconnected\n", client_id);
}


void handler_list(msg_t *msg) {
    printf("List of active clients \n\n");

    for(int i = 0; i < MAX_NO_CLIENTS; i++) {
        if(clients[i].qid != INACTIVE) {
            printf("Client %d\n", i);
        }
    }
    printf("\n\n");
}


void handler_2all(msg_t *msg) {

    for(int id = 0; id < MAX_NO_CLIENTS; id++) {
        if(clients[id].qid != INACTIVE && id != msg->id) {
            msgsnd(clients[id].qid, msg,MAX_MESSAGE_SIZE, 0);
        }
    }
}


void handler_2one(msg_t *msg) {
    int sender_id = msg->id;
    int receiver_id = msg->to_id;



    if(sender_id >= MAX_NO_CLIENTS || clients[receiver_id].qid == INACTIVE || sender_id == receiver_id) {
        fprintf(stderr, "Unable to send this message!\n");
    }
    else {
        msgsnd(clients[receiver_id].qid, msg, MAX_MESSAGE_SIZE, 0);
    }
}



void delete_queue() {
    printf("Server disconnected\n");
    fclose(file);
    msgctl(server_qid, IPC_RMID, NULL);
}


void init() {
    atexit(delete_queue);

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa, NULL);

    server_qid = open_server_queue();

    for(int id = 0; id < MAX_NO_CLIENTS; ++id)
    {
        clients[id].qid = INACTIVE;
    }
    printf("Server established!\n");

}


int main() {
    init();
    msg_t msg;

    file = fopen(filename, "w+");
    if(file == NULL) {
        fprintf(stderr, "Cannot open the file\n");
        return 1;
    }

    while(true) {
        if(msgrcv(server_qid, &msg, MAX_MESSAGE_SIZE, -100, 0) == -1){
            perror("Server unable to receive message");
            continue;
        }
        update_log(&msg);
        switch(msg.type) {
            case INIT:
                handler_init(&msg);
                break;
            case STOP:
                handler_stop(&msg);
                break;
            case LIST:
                handler_list(&msg);
                break;
            case TOALL:
                handler_2all(&msg);
                break;
            case TOONE:
                handler_2one(&msg);
                break;
        }
    }

}