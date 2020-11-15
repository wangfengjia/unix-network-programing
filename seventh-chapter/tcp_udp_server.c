//
// Created by 王勇椿 on 2020/11/15.
//
//并发TCP服务器程序和迭代UDP服务器程序组合成单个使用select来复用TCP和UDP套接字服务器程序

#include "../lib/unp.h"
#include <stddef.h>
#include <stdbool.h>

static void sig_child(int);
static void string_echo(int);

int
main(int argc, char *argv[]){

    int listenfd, connfd, udpfd, nready, maxfdp1;
    char mesg[MAXLINE];
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    socklen_t len;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;

    //create listening TCP socket
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    //create udp socket
    udpfd = Socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    Bind(udpfd, (SA *)&servaddr, sizeof(servaddr));

    Signal(SIGCHLD, sig_child);
    FD_ZERO(&rset);
    maxfdp1 = max(listenfd, udpfd) + 1;
    while (true){
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);
        if ((nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0){
            if (errno == EINTR){
                continue; //back to for()
            } else{
                err_sys("select error");
            }
        }

        if (FD_ISSET(listenfd, &rset)){
            len = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *)&cliaddr, &len);
            if ((childpid = fork()) == 0){
                Close(listenfd);
                string_echo(connfd);
                exit(0);
            }
            Close(connfd);
        }

        if (FD_ISSET(udpfd, &rset)){
            len = sizeof(cliaddr);
            n = Recvfrom(udpfd, mesg, MAXLINE, 0, (SA *)&cliaddr, &len);
            Sendto(udpfd, mesg, n, 0, (SA *)&cliaddr, len);
        }
    }

}

static void
sig_child(int signo){

    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0){
        printf("child %d terminated\n", pid);
    }
    return;
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
