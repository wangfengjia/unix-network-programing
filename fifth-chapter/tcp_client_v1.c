//
// Created by 王勇椿 on 2020/11/7.
//
#include "../lib/unp.h"
#include <stddef.h>

//从标准输入读入一行文本，发送给服务器，服务服务器对这行的返回，并把返回写到标准输出
static void string_cli(FILE *, int sockfd);

int
main(int argc, char *argv[]){

    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 0){
        err_quit("usage: tcpcli <IPaddress>");
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

    string_cli(stdin, sockfd);
    exit(0);
}

static void
string_cli(FILE *fp, int sockfd){

    char sendline[MAXLINE], recvline[MAXLINE];

    while (Fgets(sendline, MAXLINE, fp) != NULL){
        Writen(sockfd, sendline, strlen(sendline));
        if (Readline(sockfd, recvline, MAXLINE) == 0){
            err_quit("string_cli: server terminated prematurely");
        }
        fputs(recvline, stdout);
    }
}

