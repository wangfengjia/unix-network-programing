//
// Created by 王勇椿 on 2020/11/25.
//
//协议无关的时间获取服务器程序以守护进程的方式运行

#include "../lib/unp.h"
#include <stdbool.h>
#include <time.h>
#include <stddef.h>

int
main(int argc, char *argv[]){

    int listenfd, connfd;
    socklen_t addrlen, len;
    struct sockaddr *cliaddr;
    char buff[MAXLINE];
    time_t ticks;

    if (argc < 2 || argc > 3){
        err_quit("usage: daytimetcpsrv2 [ <host> ] <service or port>");
    }
    daemon_init(argv[0], 0);

    if (argc == 2){
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 3){
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
    }
    cliaddr = Malloc(addrlen);
    while (true){
        len = addrlen;
        connfd = Accept(listenfd, cliaddr, &len);
        err_msg("connection from %s", Sock_ntop(cliaddr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));
        Close(connfd);
    }
}
