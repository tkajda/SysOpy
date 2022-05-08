//
// Created by tomasz on 05.05.22.
//

#ifndef ZAD01_SHARED_H
#define ZAD01_SHARED_H

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


#define NUM_OF_COOKS 5
#define DELIVERYMEN 5
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
    int is_taken[OVEN_SIZE];
} oven;


#define rand_range(a, b) (rand() % (b + 1 - a) + a)
#define rand_sleep(a, b) usleep(rand_range(a * 1000000, b * 1000000));

#endif //ZAD01_SHARED_H
