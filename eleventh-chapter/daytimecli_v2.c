//
// Created by 王勇椿 on 2020/11/21.
//
#include "lib.h"

int
main(int argc, char *argv[]){

    int sockfd, n;
    char recvline[MAXLINE + 1];
    socklen_t socklen;
    struct sockaddr_storage ss;

    if (argc != 3){
        err_quit("usage: daytimetcpcli <hostname/IPaddress> <service/port#>");
    }

    sockfd = Tcp_connect(argv[1], argv[2]);
    socklen = sizeof(ss);
    Getpeername(sockfd, (SA *)&ss, &socklen);
    printf("connected to %s\n", Sock_ntop_host((SA *)&ss, socklen));

    while ((n = Read(sockfd, recvline, MAXLINE)) > 0){
        recvline[n] = 0; //null terminate
        Fputs(recvline, stdout);
    }

    exit(0);
}

