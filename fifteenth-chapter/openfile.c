//
// Created by 王勇椿 on 2020/12/1.
//
//打开一个文件并传递回其描述符

#include "../lib/unp.h"

int
main(int argc, char *argv[]){


    int fd;

    if (argc != 4){
        err_quit("openfile <sockfd#> <filename> <mode>");
    }
    if ((fd = open(argv[2], atoi(argv[3]))) < 0){
        exit((errno > 0) ? errno : 255);
    }
    if (write_fd(atoi(argv[1]), "", 1, fd) < 0){
        exit((errno > 0) ? errno : 255);
    }

    exit(0);
}

