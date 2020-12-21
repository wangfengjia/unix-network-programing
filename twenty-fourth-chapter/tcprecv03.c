//
// Created by 王勇椿 on 2020/12/16.
//
//使用select得到带外数据通知的接收程序(正确版)

#include "../lib/unp.h"
#include <stdbool.h>

int
main(int argc, char *argv[]){

    int listenfd, connfd, n, justreadoob = 0;
    char buf[100];
    fd_set rset, xset;

    if (argc == 2){
        listenfd = Tcp_listen(NULL, argv[1], NULL);
    } else if (argc == 3){
        listenfd = Tcp_listen(argv[1], argv[2], NULL);
    } else{
        err_quit("usage: tcprecv03 [ <host> ] <port#>");
    }

    connfd = Accept(listenfd, NULL, NULL);
    FD_ZERO(&rset);
    FD_ZERO(&xset);

    while (true){

        FD_SET(connfd, &rset);
        if (justreadoob == 0){
            FD_SET(connfd, &xset);
        }

        Select(connfd + 1, &rset, NULL, &xset, NULL);
        if (FD_ISSET(connfd, &xset)){
            n = Recv(connfd, buf, sizeof(buf) - 1, MSG_OOB);
            buf[n] = 0; //null terminated
            printf("read %d OOB byte: %s\n", n, buf);
            justreadoob = 1;
            FD_CLR(connfd, &xset);
        }

        if (FD_SET(connfd, &rset)){
            if ((n = Read(connfd, buf, sizeof(buf) - 1)) == 0){
                printf("received EOF\n");
                exit(0);
            }

            buf[n] = 0; // null terminated
            printf("read %d bytes: %s\n", n, buf);
            justreadoob = 0;
        }
    }
}
