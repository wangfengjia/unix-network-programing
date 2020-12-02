//
// Created by 王勇椿 on 2020/11/29.
//
#include "../lib/unp.h"
#include <stddef.h>

static void string_cli(FILE *, int);

int main(int argc, char *argv[]){

    int sockfd;
    struct sockaddr_un servaddr;

    sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

    string_cli(stdin, sockfd);
    exit(0);
}

static void string_cli(FILE *fp, int sockfd){

    char sendline[MAXLINE], recvline[MAXLINE];

    while (Fgets(sendline, MAXLINE, fp) != NULL){
        Writen(sockfd, sendline, strlen(sendline));
        if (Readline(sockfd, recvline, MAXLINE) == 0){
            err_quit("string_cli: server terminated prematurely");
        }
        fputs(recvline, stdout);
    }
}
