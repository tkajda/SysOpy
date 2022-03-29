//
// Created by Tomasz Kajda on 29.03.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _XOPEN_SOURCE 500
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/stat.h>
#include <dirent.h>

#define bufferSize 1024
#define COMMANDSIZE 32


void containsString(struct dirent* dir, char* filePath, char* string) {

    struct stat info;
    stat(dir->d_name,&info);

    FILE* fptr = fopen(filePath, "r");


    if(dir->d_type==DT_DIR) {
        return;
    }

    if (dir->d_type==DT_REG) {

        char* buffer = NULL;
        size_t len = 0;

        while( getline(&buffer, &len, fptr) != EOF) {
            if(strstr(buffer,string)!=NULL) {
                printf("file = %s ||||| proccess pid =%d\n", filePath, getpid());

            }
        }
        free(buffer);
        fclose(fptr);
        exit(0);

//        char command[COMMANDSIZE];
//        snprintf(command,COMMANDSIZE, "grep -c %s %s\n", string, filePath);
//        FILE *fp = popen(command, "r");
//        if (fp == NULL) {
//            perror("popen");
//            exit(EXIT_FAILURE);
//        }

    }

    fclose(fptr);

    if (fptr == NULL) {
        return ;
    }

    else{
        exit(0);
    }
}


void searchDir(char* path,char* str,int n, int MAXLEVEL) {

    n++;

    DIR * directory = opendir(path);
    if(!directory || n > MAXLEVEL) return;

    struct dirent* next_dir;

    char newPath[_POSIX_PATH_MAX];

    while((next_dir = readdir(directory))!=NULL ) {

        if ( strcmp(next_dir->d_name, ".")!=0 && strcmp(next_dir->d_name, "..")!=0) {

            strcpy(newPath,path);
            strcat(newPath,"/");
            strcat(newPath,next_dir->d_name);

            pid_t child_pid;
            child_pid = fork();

            if (child_pid==0){
                containsString(next_dir,newPath,str);
                searchDir(newPath, str, n,MAXLEVEL);
            }
        }

    }
    closedir(directory);

    exit(0);
}




int main(int argc, char** argv) {

    if (argc != 4) exit(1);

    char * endptr;
    int MAXLEVEL = strtol(argv[3], &endptr, 10);

    char* path = argv[1];
    char* str = argv[2];

    int start_depth = -1;

    searchDir(path,str,start_depth,MAXLEVEL);
//    while(wait(NULL));
}






