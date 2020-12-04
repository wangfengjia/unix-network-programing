//
// Created by 王勇椿 on 2020/12/3.
//
#include "web.h"
#include <stdbool.h>


void
home_page(const char *host, const char *fname){

    int fd, n;
    char line[MAXLINE];

    fd = Tcp_connect(host, SERV); //blocking connect
    n = snprintf(line, sizeof(line), GET_CMD, fname);
    Writen(fd, line, n);

    while (true){
        if ((n = Read(fd, line, MAXLINE)) == 0){
            break; //server closed the connection
        }
        printf("read %d bytes of home page\n", n);
    }
    printf("end-of-file on home page\n");
    Close(fd);
}