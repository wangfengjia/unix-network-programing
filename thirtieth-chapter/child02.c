//
// Created by 王勇椿 on 2020/12/26.
//
//派生各个子进程

#include "lib.h"
#include <stdbool.h>

//每个子进程调用accept返回一个已连接套接字，然后调用web_child处理客户请求，最后关闭连接，子进程一直在这个循环中往复，直到被父进程终止
static void child_main(int, int, int);

pid_t
child_make(int i, int listenfd, int addrlen){

    pid_t pid;

    if ((pid = fork()) > 0){ //parent process
        return (pid);
    }

    child_main(i, listenfd, addrlen); // never returns
}

static void
child_main(int i, int listenfd, int addrlen){

    int connfd;
    socklen_t clilen;
    struct sockaddr *cliaddr;

    cliaddr = Malloc(clilen);
    printf("child %ld starting\n", (long)getpid());

    while (true){
        clilen = addrlen;
        connfd = Accept(listenfd, cliaddr, &clilen);
        web_child(connfd);
        Close(connfd);
    }
}

