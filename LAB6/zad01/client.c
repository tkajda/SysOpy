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

    my_key = ftok(getenv("HOME"), getpid()% 'Z');


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


void handle_sigint() {
    delete_queue();
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


void init() {
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


void handle_messege_received(msg_t *msg) {
    printf("You have received message from %d\n", msg->id);
    printf("Message:\n%s\n\n", msg->text);
}



void handle_msg() {
    while(!is_empty(client_qid))  {
        msg_t msg;
        if(msgrcv(client_qid, &msg, MAX_MESSAGE_SIZE, -100, IPC_NOWAIT) != -1) {
            switch(msg.type) {
                case TOALL:
                    handle_messege_received(&msg);
                    break;
                case TOONE:
                    handle_messege_received(&msg);
                    break;
                case SHUTDOWN:
                    handle_shutdown();
                    break;
            }
        }
    }
}


// TODO
void handler_2all_send(char *text)
{
    printf("Client wants to send broadcast message!\n");
    printf("----------\n%s\n----------\n", text);
    msg_t msg;
    msg.type = TOALL;
    msg.id = id;
    sprintf(msg.text, "%s", text);
    send_to_server(&msg);
}

void handler_2one_send(char *id_and_text)
{
    msg_t msg;
    msg.type = TOONE;
    msg.id = id;

    char delim[] = " ";
    char *text = strpbrk(id_and_text, delim);
    bool empty_text = text == NULL;
    char *id_and_text_tmp = id_and_text;
    strtok_r(id_and_text, delim, &id_and_text_tmp);

    msg.to_id = atoi(id_and_text);
    if(!empty_text)
        sprintf(msg.text, "%s", text+1);

    printf("Client wants to send direct message to id -> %d\n", msg.to_id);
    printf("----------\n%s\n----------\n", msg.text);

    send_to_server(&msg);
}


void handler_list()
{
    msg_t msg;
    msg.type = LIST;
    msg.id = id;
    sprintf(msg.text, "");
    send_to_server(&msg);
}

void sender_handler_cmd(char *command, char *text)
{

    if(strcmp("STOP", command) == 0)
    {
        stop();
    }
    else if(strcmp("LIST", command) == 0)
    {
        handler_list();
    }
    else if(strcmp("2ALL", command) == 0)
    {
        handler_2all_send(text);
    }
    else if(strcmp("2ONE", command) == 0)
    {
        handler_2one_send(text);
    }
}


//END TODO

int main() {

    init();

    while(1) {
        handle_msg();

        char line[MAX_BUFF_SIZE];
        char command[MAX_BUFF_SIZE];
        char text[MAX_BUFF_SIZE];
        fgets(line, MAX_COMMAND_LEN, stdin);
        char line_cpy[256];
        strcpy(line_cpy, line);

        char *line_tmp = line_cpy;
        char *cmd = strtok_r(line_cpy, " \n", &line_tmp);

        char *text_tmp = strtok_r(NULL, "\n", &line_tmp);
        if (text_tmp == NULL)
        {
            text[0] = '\0';
        }
        else
        {
            strcpy(text, text_tmp);
        }
        if (cmd == NULL)
        {
            command[0] = '\0';
        }
        else
        {
            strcpy(command, cmd);
        }
        sender_handler_cmd(command, text);


    }


    printf("Welcome %d on server: %d\n",getpid(), server_qid);




    return 0;
}