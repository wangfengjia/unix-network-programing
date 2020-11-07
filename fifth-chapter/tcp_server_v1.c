//
// Created by 王勇椿 on 2020/11/7.
//
#include "../lib/unp.h"
#include <stddef.h>
#include <stdbool.h>

//从网络读入数据，然后返回给客户
static void string_echo(int);


int
main(int argc, char *argv[]){

    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    while (true){

        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);
        if ((childpid = fork()) == 0){ // child process
            close(listenfd);
            string_echo(connfd); // process the request
            exit(0);
        }
        close(connfd);
    }
}

static void
string_echo(int sockfd){

    ssize_t n;
    char buf[MAXLINE];

again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0){
        Writen(sockfd, buf, n);
    }
    //系统调用被信号中断
    if (n < 0 && errno == EINTR){
        goto again;
    } else if (n < 0){
        err_sys("string_echo: read error");
    }
}



