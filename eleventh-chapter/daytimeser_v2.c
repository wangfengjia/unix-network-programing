//
// Created by 王勇椿 on 2020/11/21.
//时间获取服务器程序
//
#include "lib.h"
#include <stdbool.h>
#include <stddef.h>

int
main(int argc, char *argv[]){

    int listenfd, connfd;
    socklen_t len;
    char buf[MAXLINE];
    time_t ticks;
    struct sockaddr_storage cliaddr;

    if (argc != 2){
        err_quit("usage: daytimetcpserv <service or port#>");
    }

    listenfd = Tcp_listen(NULL, argv[1], NULL);
    while (true){

        len = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *)&cliaddr, &len);
        printf("connection from %s\n", Sock_ntop((SA *)&cliaddr, len));

        ticks = time(NULL);
        snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buf, strlen(buf));

        Close(connfd);
    }

}

