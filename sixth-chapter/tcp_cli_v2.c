//
// Created by 王勇椿 on 2020/11/9.
//
//select版本的客户端，增加对批量输入的处理，当批量输入的时候可能出现这种问题:已经读取完了标准输入，但是套接字还有数据要发送和接收，第一个版本的客户端的处理中
//当读取完标准输入时，进程会终止，这样就会触发连接的关闭序列，但是这时套接字上可能还有数据要接收和发送

#include "../lib/unp.h"
#include <stddef.h>
#include <stdbool.h>
#include <sys/select.h>

static void string_cli(FILE *, int);

int
main(int argc, char *argv[]){

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

    int maxfdp1, stdineof;
    fd_set rset;
    char buf[MAXLINE];
    int n;

    stdineof = 0;
    FD_ZERO(&rset);

    while (true){

        if (stdineof == 0){
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)){ //socket readable
            if ((n = Read(sockfd, buf, MAXLINE)) == 0){
                if (stdineof == 1){
                    return; //normal termination
                } else{
                    err_quit("string_cli: server terminated prematurely");
                }
            }

            Write(fileno(stdout), buf, n);
        }
        if (FD_ISSET(fileno(fp), &rset)){ //input is readable
            if ((n = Read(fileno(fp), buf, MAXLINE)) == 0){
                stdineof = 1;
                Shutdown(sockfd, SHUT_WR); // send FIN
                FD_CLR(fileno(fp), &rset);
                continue;
            }

            Writen(sockfd, buf, n);
        }
    }
}

