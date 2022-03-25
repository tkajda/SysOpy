//
// Created by tomasz on 25.03.2022.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

double f(double x) {
    double aod = pow(x,2.0);
    return 4/(aod + 1);
}

double surface(double x0, double x1) {
    if (x1>1) {
        x1 = 1;
    }

    double mid = x1 + (x1-x0)/2;

    return f(mid)*(x1-x0);
}


int main(int argc, char**argv) {

    if (argc!=3) exit(1);

    char * endptr;
    long n = strtol(argv[2], &endptr, 10);

    double width = 1/(double)n;


    double S = 0;
    double x1 = 0;
    double x2 = x1+width;

    while (x2 < 1) {
        S += surface(x1,x2);
        x1 += width;
        x2 += width;
    }
    printf("%.15f", S);

    return 0;
}