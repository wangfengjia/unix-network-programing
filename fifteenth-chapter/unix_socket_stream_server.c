//
// Created by 王勇椿 on 2020/11/29.
//
//使用Unix域字节流协议的回射服务器程序(使用unix域套接字的回射服务器程序)

#include "../lib/unp.h"
#include <stdbool.h>

static void sig_chld(int);
static void string_echo(int);

int
main(int argc, char *argv[]){

    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_un cliaddr, servaddr;

    listenfd = Socket(AF_LOCAL, SOCK_STREAM, 0);
    unlink(UNIXSTR_PATH);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);
    Signal(SIGCHLD, sig_chld);


    while (true){

        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (SA *)&cliaddr, &clilen)) < 0){
            if (errno == EINTR){
                continue;
            } else{
                err_sys("accept error");
            }
        }
        if ((childpid = fork()) == 0){ // child process
            Close(listenfd);
            string_echo(connfd);
            exit(0);
        }
    }
}

static void
sig_chld(int signo){

    pid_t pid;
    int stat;

    //当有多个客户端并发发送EOF字符到服务器，这样就就会有多个子进程终止，这样就可能出现多个信号在信号处理函数执行之前产生，但是当调用wait函数的时候，只执行一次
    //只有一个SIGCHLD信号被处理，其他产生SIGCHLD信号未被处理的子进程被变为僵死状态，通过在循环中调用waitpid来解决这个问题，循环中调用waitpid函数可以获取所有
    //已终止子进程的状态，waitpid函数的WNOHANG参数的作用是告知waitpid调用时没有终止的子进程时不要阻塞
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



