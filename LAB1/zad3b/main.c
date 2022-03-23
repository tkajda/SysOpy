//
// Created by Tomasz Kajda on 12.03.2022.
//
#include<stdio.h>
#include "library.c"
#include "time.h"
#include <sys/times.h>


#define DELANDRESTORE 70


double caclucateTime(clock_t start, clock_t end) {
    return (double)(end - start)/CLOCKS_PER_SEC;
}



int main(int argc, char **argv) {
    

    clock_t start = clock();

    char* possibleFunctions[] = { "create_table", "wc_files", "remove_block"};
    ArrayOfBlocks * arr = createTable(argc-2);

    if(strcmp(possibleFunctions[0], argv[1]) == 0) {
        printf("Array of size %d allocated successfully\n", argc-2);

    }
    else if(strcmp(possibleFunctions[1], argv[1]) == 0) {

        fileToArray(arr, argv,argc);
//        printMainArray(arr, argc-2);
    }
    else if(strcmp(possibleFunctions[2], argv[1]) == 0) {
        printf("Test deletes random file's data and replace it with random file from arg list several times\n");
        fileToArray(arr, argv,argc);
        for(int j = 0; j <DELANDRESTORE; j++) {
            int x = (rand()%(argc-2)) +2;

            deleteIndex(arr, x);
            int r = (rand()%(argc-2))+2;

            createBlock(arr,argv[r]);
        }
//        printMainArray(arr, argc-2);
    }
    else {
        printf("unknown function\n");
        exit(1);
    }


    struct tms **tms_time = calloc(6,  sizeof(struct tms *));
    for (int i = 0; i < 6; i++) {
        tms_time[i] = (struct tms *) calloc(1,sizeof(struct tms ));
    }


    clock_t end = clock();

    long int totalLines = 0;
    for(int i = 0; i < argc-2;i++) {
        if(arr->blocks[i] != NULL) {
            totalLines +=  strtol(arr->blocks[i]->lineCount,NULL,10);
        }
    }

    printf("number of files: %d with total number of lines %ld\n", argc-2, totalLines);
    printf("User time: %.10f\n", caclucateTime(tms_time[0]->tms_utime, tms_time[1]->tms_utime));
    printf("System time: %.10f\n", caclucateTime(tms_time[0]->tms_stime,tms_time[1]->tms_stime));
    printf("CPU time: %.10f seconds.\n\n", caclucateTime(start,end));


    free(arr);


    return 0;
}

