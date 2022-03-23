//
// Created by Tomasz Kajda on 21.03.2022.
//

#define  _GNU_SOURCE
#define  _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/times.h>

#define MAX 256

struct results count_lib(char*, char*);
struct results count_sys(char*, char*);



double calcucateTime(clock_t start, clock_t end) {
    return (double)(end - start)/CLOCKS_PER_SEC;
}

void printTimes(struct tms** tms_time, clock_t start, clock_t end) {
    if(tms_time) {

        printf("User time: %.10f\n", calcucateTime(tms_time[0]->tms_utime, tms_time[1]->tms_utime));
        printf("System time: %.10f\n", calcucateTime(tms_time[0]->tms_stime,tms_time[1]->tms_stime));
        printf("CPU time: %.10f seconds.\n\n", calcucateTime(start,end));
    }
}



struct results {
    size_t line_count;
    size_t char_count;
    size_t filelen;
} results;



int main(int argc, char** argv) {

    if (argc!=3) return 1;

    struct tms **tms_time = calloc(6,  sizeof(struct tms *));
    struct tms **tms_time1 = calloc(6,  sizeof(struct tms *));


    for (int i = 0; i < 6; i++) {
        tms_time[i] = (struct tms *) calloc(1,sizeof(struct tms *));
        tms_time1[i] = (struct tms *) calloc(1,sizeof(struct tms *));
    }

    char* sign = argv[1];
    char* filename = argv[2];

    printf("using library\n");

    clock_t start = clock();
    struct results result = count_lib(filename,sign);
    clock_t end = clock();

    printf("\nfile: %s, length of the file: %zu lines, char: %s\n",
           filename,result.filelen, sign);
    printTimes(tms_time, start,end);


    printf("using system\n");
    clock_t start1 = clock();
    struct results results1 = count_sys(filename,sign);
    clock_t end1 = clock();
    printTimes(tms_time1, start1,end1);
    printf("\n\n\n");


    free(tms_time);

    return 0;
}

struct results count_lib(char* filename, char* sign) {

    FILE* fp =fopen(filename,"r");

    if (!fp) {
        printf("cannot read file");
        exit(1);
    }


    size_t line_counter = 0;
    size_t char_counter = 0;
    size_t file_len = 0;


//    size_t len = 0;
//    size_t read;
//    char* line = NULL;
//    while((read=getline(&line, &len, fp)!=-1)) {
//
//        if(strcmp(line,sign) != 0) {
//            line_counter++;
//            for(int i = 0; i < strlen(line); i++) {
//                if(line[i]==sign[0]) {
//                    char_counter++;
//                }
//            }
//        }
//        file_len++;
//
//    }

    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    char buffer[file_size];
    rewind(fp);
    fread(buffer, 1, MAX, fp);

    int i =0;
    short is_in_line = 0;

    for (int i = 0; i<= file_size; i++) {
        if (buffer[i] == '\n') {
            file_len++;
            if (is_in_line == 1) {
                line_counter ++;
            }
            is_in_line = 0;
        }
        else if (buffer[i] == sign[0]) {
            is_in_line=1;
            char_counter ++;
        }
    }

    fclose(fp);

    struct results res;
    res.char_count = char_counter;
    res.line_count = line_counter;
    res.filelen = file_len;
    return res;
}


struct results count_sys(char* filename, char* sign) {
    int file = open(filename, O_RDONLY);


    if (!file) {
        printf("cannot read file");
        exit(1);
    }


    size_t file_size = lseek(file, 0, SEEK_END);
    char buffer[file_size];
    read(file, buffer, sizeof(char) * MAX);
    lseek(file,0,SEEK_SET);

    size_t char_counter = 0;
    size_t line_counter = 0;
    size_t file_lines = 0;
    short is_in_line = 0;

//
//    char c;
//    while (read(file, &c, 1) != -1) {
//        if (c == sign[0]) char_counter++;
//        if (c == '\n') line_counter++;
//    }


    for (int i = 0; i<= file_size; i++) {
        if (buffer[i] == '\n') {
            file_lines++;
            if (is_in_line == 1) {
                line_counter ++;
            }
            is_in_line = 0;
        }
        else if (buffer[i] == sign[0]) {
            is_in_line=1;
            char_counter ++;
        }
    }

    struct results res;

    res.char_count = char_counter;
    res.line_count = line_counter;
    res.filelen = file_lines;

    return res;


}