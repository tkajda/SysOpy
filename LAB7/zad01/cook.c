//
// Created by tomasz on 05.05.22.
//


#include "shared.h"



int main() {

    srand(time(NULL)  ^ (getpid()<<16) );
    fflush(stdout);
    int pizza_type = rand() % 10;

    printf("(%d %ld) Przygotowuje pizze: %d\n", getpid(), time(NULL), pizza_type);
    fflush(stdout);


    return 0;
}