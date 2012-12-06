#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>

#define OPT_SYM 0 


int main(int argc, char *argv[])
{
    int flags = 0, opt;
    char * newpath;
    char * oldpath;
    while((opt = getopt(argc, argv, "s")) != -1){
        switch(opt) {
            case 's':
                flags |= 1<<OPT_SYM;
                break;
            default:
                fprintf(stderr, "Usage: %s [-s] oldpath newpath\n", argv[0]);
                exit(1);
        }
    }
    if( argc - optind == 2){
        oldpath = argv[optind];
        newpath = argv[optind+1];
    } else {
        fprintf(stderr, "%s: missing file operand\nUsage: %s [-s] oldpath newpath\n", argv[0], argv[0]);
        exit(1);
    }

    if(flags == 1<<OPT_SYM){
        if(symlink(oldpath, newpath) < 0){
            perror(argv[0]);
            exit(1);
        }
    } else {
        if(link(oldpath, newpath) < 0){
            perror(argv[0]);
            exit(1);
        }
    }
    return 0;
} 

