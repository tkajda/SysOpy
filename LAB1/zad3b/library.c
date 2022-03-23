#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 128
#define COMMANDSIZE 20
#define BASE 10

void printMainArray(ArrayOfBlocks *array, int n) {

    int index = 0;

    while(index < n) {
        if(array->blocks[index] != NULL){
            printf("\ncharCount=%slineCount=%swordCount=%s\n",
                   array->blocks[index]->charCount,
                   array->blocks[index]->lineCount,
                   array->blocks[index]->wordCount);
        }
        index++;
    }

}



struct ArrayOfBlocks* createTable(int numberOfBlocks) {

    ArrayOfBlocks * t = calloc(1, sizeof(ArrayOfBlocks));
    t->size = numberOfBlocks;
    t->blocks = calloc(numberOfBlocks, sizeof(Block*));
    return t;

}




char *count(char *file, char* thing) {

    char* command = calloc(COMMANDSIZE, sizeof(char));

    strcat(command, "wc -");
    strcat(command, thing);
    strcat(command, " < ");
    strcat(command, file);

    FILE *fp=popen(command, "r");

    free(command);

    char buf[BUFSIZE];

    char* result=calloc(BUFSIZE, sizeof(char));

    if (fp == NULL) {
        exit(1);
    }

    while (fgets(buf, BUFSIZE, fp) != NULL) {
        strcat(result,buf);
        return result;
    }

    if (pclose(fp)) {
        printf("error\n");
    }

//    system(command);
    free(result);
    return 0;
}


int addBlockToArray(ArrayOfBlocks* arr, struct Block* block ) {

    int i = -1;
    while(arr->blocks[++i] != NULL);
    arr->blocks[i] = block;
    arr->size++;

    return i;
}

void createBlock(ArrayOfBlocks *arr,char *filename) {

    Block* ptr = calloc(1, sizeof(Block));
    ptr->lineCount = count(filename, "l");
    ptr->wordCount = count(filename, "w");
    ptr->charCount = count(filename, "c");
    addBlockToArray(arr, ptr);
}





void deleteIndex(ArrayOfBlocks* array, int index) {
    if(array->blocks[index]==NULL || index >array->size) {
        return;
    }
    array->blocks[index]=NULL;
    free(array->blocks[index]);
    array->size--;

}




void fileToArray(struct ArrayOfBlocks* array, char** filenames,int n) {

    FILE* tmp = tmpfile();
    if (tmp == NULL) {
        return ;
    }
//    puts("tmp file created");


    for(int i = 2 ; i < n;i++) {
        char* line = calloc(BUFSIZE, sizeof(char));

        strcat(line,count(filenames[i], "l"));
        strcat(line,count(filenames[i], "w"));
        strcat(line,count(filenames[i], "c"));

        fputs(line, tmp);
        free(line);
    }

    rewind(tmp);

    addFromFileToArray(tmp, n, array);
    fclose(tmp);

}


void addFromFileToArray(FILE *tmp, int n,struct ArrayOfBlocks* array) {

    fseek(tmp, 0, SEEK_END);
    fseek(tmp, 0, SEEK_SET);
    size_t len = 0;


    int i = -1;

    char** arrayOfCounters = calloc(3*n, sizeof(char*));


    while (!feof(tmp)) {
        char * line = calloc(100, sizeof(char));
        getline(&line, &len, tmp);
        if (line!=NULL) {
            i++;
            arrayOfCounters[i] = line;
        }
    }

    for(int j = 0; j < 3*n; j += 3) {
        Block* ptr = calloc(1, sizeof(Block));
        ptr->lineCount = arrayOfCounters[j];
        ptr->wordCount = arrayOfCounters[j+1];
        ptr->charCount = arrayOfCounters[j+2];

        addBlockToArray(array,ptr);

    }

    free(arrayOfCounters);

}




