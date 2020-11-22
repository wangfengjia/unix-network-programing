//
// Created by 王勇椿 on 2020/11/21.
//
//通过命令行参数指定网络层的协议:IPv4还是IPv6

#include "../lib/unp.h"
#include <time.h>
#include <stdbool.h>
#include <stddef.h>

int
main(int argc, char *argv[]){

    int listenfd, connfd;
    socklen_t addrlen, len;
    char buf[MAXLINE];
    time_t ticks;
    struct sockaddr_storage cliaddr;

    if (argc == 2){
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);
    }else if (argc == 3){
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
    } else{
        err_quit("usage: daytimetcpserv [ <host> ] <service or port#>");
    }

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



