//
// Created by 王勇椿 on 2020/11/7.
//
#include "../lib/unp.h"

int
main(int argc, char *argv[]){

    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE];

    if (argc != 2){
        err_quit("usage: tcpcli <IPaddress>");
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

    sendline[0] = 'a';
    Writen(sockfd, sendline, strlen(sendline));
    sleep(3000);
//    string_cli(stdin, sockfd);
    exit(0);
}

