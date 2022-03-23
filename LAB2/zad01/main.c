//
// Created by Tomasz Kajda on 20.03.2022.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/times.h>

#define MAX 1000

void using_lib(char*, char*);
void using_sys(char*, char*);


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



int main(int argc, char** argv) {

    char* filename1;
    char* filename2;

    struct tms **tms_time = calloc(6,  sizeof(struct tms *));
    struct tms **tms_time1 = calloc(6,  sizeof(struct tms *));


    for (int i = 0; i < 6; i++) {
        tms_time[i] = (struct tms *) calloc(1,sizeof(struct tms *));
        tms_time1[i] = (struct tms *) calloc(1,sizeof(struct tms *));
    }


    switch (argc) {
        case 3:
            filename1 = argv[1];
            filename2 = argv[2];
            break;
        case 2:
            filename1=argv[1];
            filename2=calloc(100,sizeof(char));
            printf("\nEnter file name: ");
            scanf("%s", filename2);
            break;
        case 1:
            filename1=calloc(100,sizeof(char));
            filename2=calloc(100,sizeof(char));
            printf("\nEnter file(s) name(s): ");
            scanf("%s %s", filename1, filename2);
            break;
        default:
            printf("invalid number of files\n");
            exit(1);
    }

    printf("\nfiles: %s %s\n", filename1, filename2);
    printf("Using library\n");

    // using library time
    clock_t start = clock();
    using_lib(filename1,filename2);
    clock_t end = clock();

    printTimes(tms_time, start,end);
    free(tms_time);


    //using system time
    printf("Using system functions\n");
    clock_t start1 = clock();
    using_sys(filename1,filename2);
    clock_t end1 = clock();

    printTimes(tms_time1, start1,end1);
    free(tms_time1);


    return 0;
}




short isWhitespaceString(char* str) {
    while (isspace(*str)) { ++str; }

    if (*str == 0) {
        return 1;
    }

    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) { --end; }

    return isspace(*end);
}

char* rewrite(char* buffer, size_t baits, char* output) {

    int output_index = 0;

    for(int i = 0; i < baits; i ++ ) {
        int j = 0;
        int x = i;
        char* line = calloc(MAX, sizeof(char));

        while(buffer[x]!='\n') {
            line[j++] = buffer[x++];
            i++;
        }

        if (!isWhitespaceString(line) && j > 0) {
            int iter=0;
            while(line[iter]) {
                output[output_index++] = line[iter++];
            }
            if (i < baits) {
                output[output_index++] = '\n';
            }
        }
        free(line);
    }
    return output;

}





void using_lib(char* readFileName, char* writeFileName) {

    FILE * file1=fopen(readFileName, "r");
    FILE* file2=fopen(writeFileName, "w+");


    if(!file1) {
        printf("cannot read file");
        exit(1);
    }
    if(!file2) {
        printf("cannot write to file");
        exit(1);
    }


    char *buffer = calloc(MAX, sizeof(char));
    size_t baits;
    baits = fread(buffer, sizeof(char), MAX, file1);
    char* output = calloc(MAX, sizeof(char));

    output = rewrite(buffer, baits, output);
    int i = 0;
    while(output[i] && i <strlen(output)) {
        fwrite(&output[i],sizeof(char),1,file2);

        i++;
    }

    free(output);
    fclose(file1);
    fclose(file2);

}



void using_sys(char * readFileName, char* writeFileName) {
    int file1=open(readFileName, O_RDONLY);
    int file2=open(writeFileName, O_WRONLY);


    if(!file1) {
        printf("cannot read file");
        exit(1);
    }
    if(!file2) {
        printf("cannot write to file");
        exit(1);
    }

    char* buffer = calloc(MAX,sizeof(char));
    ssize_t baits = read(file1, buffer, sizeof(char)*MAX);

    char* output = calloc(MAX,sizeof(char));
    output = rewrite(buffer, baits, output);

    int i = 0;

    while(output[i] && i <strlen(output)) {
        write(file2, &output[i], 1);
        i++;
    }
    free(output);
    close(file1);
    close(file2);



}








