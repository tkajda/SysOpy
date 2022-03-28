//
// Created by tomasz on 25.03.2022.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

#define MAX 32

long double  f(long double  x) {
    long double  aod = pow(x,2.0);
    return 4/(aod + 1);
}

long double  surface(double x0, double x1) {
    if (x1>1) {
        x1 = 1;
    }

    long double  mid = x1 + (x1-x0)/2;
    return f(mid)*(x1-x0);
}


int main(int argc, char**argv) {

    if (argc!=3) exit(1);

    char * endptr;
    long n = strtol(argv[2], &endptr, 10);

    long double  width = 1/(long double )n;

    long double  x1 = 0;
    long double  x2 = x1+width;

    for(int i = 0; i<n; i++){

        pid_t child_pid;
        child_pid = fork();
        if (child_pid == 0) {
            char fileName[MAX];
            snprintf(fileName,MAX,"../zad02/result%d.txt",i+1);
            FILE* fptr = fopen(fileName,"w");
            fprintf(fptr, "%Lf", surface(x1,x2));
            fclose(fptr);
            exit(0);
        }

        x1 += width;
        x2 += width;
    }
    for(int i=0;i<n;i++)
        wait(NULL);

    long double S = 0, buff;
    for(int i=0;i<n;i++){
        char fileName[MAX];
        snprintf(fileName,MAX,"../zad02/result%d.txt",i+1);
        FILE* fptr = fopen(fileName,"r");
        fscanf(fptr,"%Lf",&buff);
        fclose(fptr);
        S += buff;
    }

    printf("%.15Lf", S);


    return 0;
}