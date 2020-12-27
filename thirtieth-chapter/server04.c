//
// Created by 王勇椿 on 2020/12/26.
//
//TCP并发服务器程序:为每个客户创建一个线程
#include "lib.h"

static void sig_int(int);
static void *doit(void *);

static void
sig_int(int signo){

    pr_cpu_time();
    exit(0);
}

static void *
doit(void *arg){

    Pthread_detach(pthread_self());
    web_child((int) arg);
    Close((int) arg);
    return (NULL);
}

int
main(int argc, char *argv[]){

    int listenfd, connfd;
    pthread_t tid;
    socklen_t clilen, addrlen;
    struct sockaddr *cliaddr;

    if (argc == 2){
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 3){
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
    } else{
        err_quit("usage: serv04 [ <host> ] <port#>");
    }

    cliaddr = Malloc(addrlen);
    Signal(SIGINT, sig_int);
    while (true){

        clilen = addrlen;
        connfd = Accept(listenfd, cliaddr, &clilen);
        Pthread_create(&tid, NULL, &doit, (void *) connfd);
    }
}



