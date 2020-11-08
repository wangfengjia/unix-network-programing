//
// Created by 王勇椿 on 2020/11/8.
//
//select版本的TCP客户端
#include "../lib/unp.h"
#include <stddef.h>
#include <stdbool.h>
#include <sys/select.h>

static void string_cli(FILE *, int sockfd);

int main(int argc, char *argv[]){

    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2){
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

    int maxfd1;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];

    FD_ZERO(&rset);
    while (true){

        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfd1 = max(fileno(fp), sockfd) + 1;
        Select(maxfd1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)){ //socket is readable
            if (Readline(sockfd, recvline, MAXLINE) == 0){
                err_quit("string_cli: server terminated prematurely");
            }
            Fputs(recvline, stdout);
        }
        if (FD_ISSET(fileno(fp), &rset)){ //input readable
            if (Fgets(sendline, MAXLINE, fp) == NULL){
                return; //all done
            }
            Writen(sockfd, sendline, strlen(sendline));
        }
    }
}
