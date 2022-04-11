//
// Created by Tomasz Kajda on 11.04.2022.
//
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 128
#define MAX 64

int execute_commands(char** commands, char* flow) {

    int i = 0;
    int pipesfd[MAX][2];
    int cnt = 0;
    for (int x = 0; x < MAX; x ++) {
        if (pipe(pipesfd[x]) == -1) {
            printf("cannot pipe");
            return 1;
        }
    }

    while (i<strlen(flow)) {
        if (flow[i] != ' ' && flow[i] != '|') {
            cnt++;

            pid_t pid = fork();

            if (pid == 0 ) {
                printf("%d\n", cnt);
                int commNum = atoi(&flow[i]) - 1;
                for (int j = 0; j < MAX; j++) {
                    close(pipesfd[j][0]);
                }

                if (i>0) {
                    dup2(pipesfd[cnt-1][0], STDIN_FILENO);
                }
                if (i+1>=strlen(flow)) {
                    dup2(pipesfd[cnt][0], STDOUT_FILENO);
                }
//                printf("%d   %s\n",commNum, commands[commNum]);
                int err = execl(commands[commNum],commands[commNum]);
                exit(err);
            }
            else {
                wait(NULL);
            }
        }
        i++;
    }

    for (int x = 0; x < MAX; ++x) {
        wait(0);
    }

    for (int j = 0; j < MAX; j++){
        close(pipesfd[j][0]);
        close(pipesfd[j][1]);
    }
    for (int i = 0; i < MAX; i++){
        printf("%d %d\n", pipesfd[i][0], pipesfd[i][1]);
    }



//    char *output;
//    read(pipesfd[cnt-1][0], &output, 8);
//    printf("%s",output);

    return 0;
}


int main(int argc, char** argv) {

    if (argc != 3) return 1;

    FILE* fp = fopen(argv[1], "r");
    char * line = NULL;
    size_t len = 0;


    char *commands[100];

    if (fp == NULL)
        exit(EXIT_FAILURE);

    int i = 0;

    while ((getline(&line, &len, fp)) != -1) {
        commands[i] = line;
        i++;
        line = calloc(SIZE, sizeof(char));
    }
    free(line);
    fclose(fp);
    char *flow = argv[2];

    execute_commands(commands, flow);
    printf("task finished\n");
    return 0;
}


