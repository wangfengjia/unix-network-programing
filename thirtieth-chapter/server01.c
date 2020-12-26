//
// Created by 王勇椿 on 2020/12/26.
//
/**
 * TCP并发服务器程序，每个客户一个子进程。服务器可以同时服务的客户数目的限制是操作系统对运行服务器程序的进成的用户ID能够同时拥有多少子进程限制
 * 缺点:为每个客户现场fork一个子进程很消耗CPU时间
 */

#include "lib.h"

//捕获由键入终端中断键产生的SIGINT信号，在客户运行完毕之后键入中断键以显示服务器程序运行所花的CPU时间
void sig_int(int);
void sig_chld(int);

void
sig_int(int signo){

    pr_cpu_time();
    exit(0);
}

void
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

int main(int argc, char *argv[]){

    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen, addrlen;
    struct sockaddr *cliaddr;

    if (argc == 2){
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 3){
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
    } else{
        err_quit("usage: serv01 [ <host> ] <port#>");
    }

    cliaddr = Malloc(clilen);
    Signal(SIGCHLD, sig_chld);
    Signal(SIGINT, sig_int);

    while (true){
        clilen = addrlen;
        if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0){
            if (errno == EINTR){
                continue;
            } else{
                err_sys("accept error");
            }
        }

        //child process
        if ((childpid = fork()) == 0){
            Close(listenfd); // close listening socket
            web_child(connfd); // process request
            exit(0);
        }

        Close(connfd); //parent close connected socket
    }

}




