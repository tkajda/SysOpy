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
#include <sys/mman.h>

#define NUM_OF_COOKS 6
#define DELIVERYMEN 8
#define TABLE_SIZE 5
#define OVEN_SIZE 5

#define TABLE_SHARED_MEMORY "/TABLE_SHARED_MEMORY"
#define OVEN_SHARED_MEMORY "/OVEN_SHARED_MEMORY"
#define TABLE_SEMAPHORE "/TABLE"
#define OVEN_SEMAPHORE "/OVEN"


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

#endif //LAB7_SHARED_H
