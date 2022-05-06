//
// Created by tomasz on 06.05.22.
//

#ifndef LAB7_SHARED_H
#define LAB7_SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>


#define NUM_OF_COOKS 2
#define DELIVERYMEN 2
#define TABLE_SIZE 5
#define OVEN_SIZE 5

#define PATHNAME getenv("HOME")

typedef struct sembuf sembuf;


typedef struct {
    int pizzas[TABLE_SIZE];
    int last_taken ;
    int is_taken[TABLE_SIZE];

} table;

typedef struct {
    int pizzas[OVEN_SIZE];
    int last_taken;

} oven;


#endif //LAB7_SHARED_H
