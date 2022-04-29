//
// Created by tomasz on 29.04.22.
//

#ifndef ZAD01_HEADER_H
#define ZAD01_HEADER_H

#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>

#define STOP 1
#define LIST 2
#define INIT 3
#define TOALL 4
#define TOONE 5
#define SHUTDOWN 6


#define MAX_BUFF_SIZE 2048
#define MAX_MESSEGE_LENGTH 256

typedef struct msg_t
{
    long type;
    time_t timestamp;
    int id;
    int to_id;
    char text[MAX_MESSEGE_LENGTH];
    int pid;
} msg_t;

#define MAX_MESSAGE_SIZE (sizeof(msg_t) - sizeof(long))
#define MESSAGE_SIZE sizeof(msg_t)
#define MAX_MESSAGES 10

#endif