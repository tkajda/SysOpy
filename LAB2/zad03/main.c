//
// Created by Tomasz Kajda on 21.03.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include "dirent.h"
#include "string.h"

#define _XOPEN_SOURCE 500
#define __USE_XOPEN_EXTENDED 1
#include <ftw.h>



#ifndef _POSIX_PATH_MAX
#define _POSIX_PATH_MAX  512
#endif


struct results {
    int files;
    int catalogs;
    int special_files;
    int block_files;
    int fifos;
    int sym_links;
    int sockets;
} results, results_nftw;


char* findType(unsigned char type) {
    switch(type) {
        case DT_REG :
            results.files++;
            return "zwykly plik";
        case DT_BLK :
            results.block_files++;
            return "specjalny plik blokowy";
        case DT_DIR:
            results.catalogs++;
            return "katalog";
        case DT_FIFO:
            results.fifos++;
            return "FIFO";
        case DT_SOCK:
            results.sockets++;
            return "socket";
        case DT_LNK:
            results.sym_links++;
            return "link symboliczny";
        case DT_CHR:
            results.special_files++;
            return "specjalny plik znakowy";

    }
    return "unknown";
}



void printInfo(struct dirent* dir, char* path) {

    struct stat info;
    stat(dir->d_name,&info);

    printf("Sciezka bezwzgledna: %s \n"
           "Liczba dowiazan: %lu \n"
           "Rodzaj: %s\n"
           "Rozmiar: %ld\n"
           "Data ostatniego dostepu: %s.\n"
           "Data ostatniej modyfikacji: %s\n",
           realpath(path, NULL),
           info.st_nlink,
           findType(dir->d_type),
           info.st_size,
           ctime(&info.st_atime),
           ctime(&info.st_mtime));

}


void using_stat(char* path){
    printf("%s\n\n", path);

    DIR * directory = opendir(path);
    if(!directory) return;
    struct dirent* next_dir;

    char newPath[_POSIX_PATH_MAX];
    while((next_dir = readdir(directory))!=NULL) {
        if ( strcmp(next_dir->d_name, ".")!=0 && strcmp(next_dir->d_name, "..")!=0) {
            strcpy(newPath,path);
            strcat(newPath,"/");
            strcat(newPath,next_dir->d_name);
            printInfo(next_dir,newPath);

            using_stat(newPath);
        }

    }
    closedir(directory);

}



int nftw_function(char *path, struct stat *statPtr, int type, struct FTW *pfwt) {

    char *ptr =realpath(path, NULL);

    char* name = NULL;

    switch (type) {
        case FTW_F:
            switch (statPtr->st_mode & S_IFMT) {
                case S_IFREG:
                    name = "zwykly plik";
                    results_nftw.files++;
                    break;
                case S_IFBLK:
                    name = "plik blokowy";
                    results_nftw.block_files++;
                    break;
                case S_IFCHR:
                    name = "specjalny plik tekstowy";
                    results_nftw.special_files++;
                    break;
                case S_IFIFO:
                    name = "FIFO";
                    results_nftw.fifos++;
                    break;
                case S_IFLNK:
                    name = "symboliczny link";
                    results_nftw.sym_links++;
                    break;
                case S_IFSOCK:
                    name = "socket";
                    results_nftw.sockets++;
                    break;
                default:
                    printf("unknown?\n");
                    break;
            }
            break;

        case FTW_D :
            name = "katalog";
            results_nftw.catalogs++;
            break;

    }

    printf("Sciezka bezwzgledna: %s \n"
           "Liczba dowiazan: %lu \n"
           "Rodzaj: %s\n"
           "Rozmiar: %ld\n"
           "Data ostatniego dostepu: %s.\n"
           "Data ostatniej modyfikacji: %s\n",
           ptr,
           statPtr->st_nlink,
           name,
           statPtr->st_size,
           ctime(&statPtr->st_atime),
           ctime(&statPtr->st_mtime));

    return 0;
}

void printResults(struct results* result) {
    printf("\nZnaleziono: \n"
           "Regularne pliki: %d\n"
           "Katalogi: %d\n"
           "Specjalne pliki znakowe: %d\n"
           "Specjalne pliki blokowe: %d\n"
           "FIFO: %d\n"
           "Sockety: %d\n"
           "Linki symboliczne: %d\n",
           result->files,
           result->catalogs,
           result->special_files,
           result->block_files,
           result->fifos,
           result->sockets,
           result->sym_links);

}





int main(int argc, char** argv) {

    if (argc<2) exit(1);

    char* path = argv[1];

    // using system stat
    using_stat(path);
    printResults(&results);


    // using nftw
    int fd_limit = 5;
    int flags = FTW_PHYS;
    void*f = nftw_function;

    nftw(path, f, fd_limit, flags);

    printResults(&results_nftw);
    return 0;
}
