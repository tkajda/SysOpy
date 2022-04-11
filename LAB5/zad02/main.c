//
// Created by Tomasz Kajda on 11.04.2022.
//
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 1024

int main(int argc, char** argv) {

    if (argc==4) {

        char* command = calloc(MAX, sizeof(char));
        char* email = argv[1];
        char* title = argv[2];
        char* content = argv[3];

//        sprintf(command, "echo \"%s\" | mail -s \"%s\" %s ",content,title,email  );
        sprintf(command, " mail -s \"%s\" %s << %s",title,email,content );
        FILE* fp;

        if (!(fp = popen(command, "w"))) {
            perror("cannot read file");
            return 1;
        };
        pclose(fp);

        printf("email sent\n\n");
        free(command);

    }
    if (argc==2) {

        char* item = argv[1];
        int i =0;
        int date = -1;
        char* command = calloc(MAX, sizeof(char));
        FILE* fp;


        while(i++<strlen(item)) {
            if (atoi(&item[i]) != 0) {
                date = 1;
                break;
            }
        }
        if (date == 1) {
            sprintf(command, "mail -H | sort -k 7 | sort -k 6 | sort -k 5");

        }
        else {
            sprintf(command,"mail -H | sort -k 3");
        }
        fp = popen(command, "r");

        char* line = calloc(MAX,sizeof(char));
        int len = 0;

        while(getline(&line, &len, fp) != EOF ){
            printf("%s", line);
            line = calloc(MAX,sizeof(char));
        }
        free(line);
        free(command);
    }


    return 0;
}
