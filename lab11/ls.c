#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <getopt.h>

#define OPT_SIZE 0
#define OPT_INODE 1
#define OPT_FTYPE 2

int main(int argc, char *argv[])
{
    int flags = 0, opt;
    char * dirname = ".";
    while((opt = getopt(argc, argv, "siF")) != -1){
        switch(opt) {
            case 's':
                flags |= 1<<OPT_SIZE;
                break;
            case 'i':
                flags |= 1<<OPT_INODE;
                break;
            case 'F':
                flags |= 1<<OPT_FTYPE;
                break;
            default:
                fprintf(stderr,
                        "Usage: %s [OPTIONS]\n -s\t\tsize\n -i\t\tinode\n -F\t\tfiletype\n", 
                        argv[0]);
                exit(1);
        }
    }
    if(optind < argc){
        dirname = argv[optind];
    }
    DIR *dirPtr;
    struct dirent *entryPtr;
    struct stat stbuf;

    if((dirPtr = opendir(dirname)) == NULL){
        perror(dirname);
        exit(1);
    }
    char rel_fname[4096];
    while ((entryPtr = readdir (dirPtr))){
        strcpy(rel_fname, dirname);
        strcat(rel_fname, "/");
        strcat(rel_fname, entryPtr->d_name);
        if(stat(rel_fname, &stbuf) < 0){
            perror(entryPtr->d_name);
            exit(1);
        }
        if(flags & 1<<OPT_INODE){
            printf("%lu\t", (unsigned long)stbuf.st_ino);
        }
        if(flags & 1<<OPT_SIZE){
            printf("%lu\t", (unsigned long)stbuf.st_blocks);
        }
        int cchar = ' ';
        //if(S_ISREG(stbuf.st_mode))
        //    cchar = ' ';
        //else
        if(stbuf.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
            cchar = '*';
        if(S_ISDIR(stbuf.st_mode))
            cchar = '/';
        if(S_ISLNK(stbuf.st_mode))
            cchar = '@';
        if(S_ISFIFO(stbuf.st_mode))
            cchar = '|';

        printf("%s%c\n", entryPtr->d_name, cchar);
    }
    closedir (dirPtr);
    return 0;
} 

