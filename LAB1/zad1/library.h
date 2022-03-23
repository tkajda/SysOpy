#ifndef UNTITLED4_LIBRARY_H
#define UNTITLED4_LIBRARY_H


#include <bits/types/FILE.h>

typedef struct Block {
    char* filename;
    char* lineCount;
    char* wordCount;
    char* charCount;
} Block;

typedef struct ArrayOfBlocks {
    int size;
    Block** blocks;
} ArrayOfBlocks;

void printMainArray(ArrayOfBlocks*, int );
char *count(char*, char*);
struct ArrayOfBlocks* createTable(int);
struct Block* count_all(char*);
int addBlockToArray(ArrayOfBlocks*, Block*);
void deleteIndex(ArrayOfBlocks *, int );
void fileToArray(ArrayOfBlocks*,char**,int);
void addFromFileToArray(FILE*, int, ArrayOfBlocks*);

#endif //UNTITLED4_LIBRARY_H
