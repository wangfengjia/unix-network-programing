//
// Created by 王勇椿 on 2020/12/26.
//
/**
 * 第二版的服务器程序:使用子进程池的方案。在服务器程序的启动阶段预先派生一定数量的子进程，当各个客户连接到达时，这些子进程就能为这些客户服务
 * 优点:无须引入父进程执行fork的开销就能处理新客户的请求
 * 缺点:
 *      1. 父进程必须在服务器的启动阶段预估需要预先派生多少子进程。当这个数量预估不准的时候，新到的客户请求将不能得到立刻处理，直到有空闲的子进程，
 *      这样的话对于这个新到的客户的响应时间将会比较慢
 *      2.由于服务器在启动阶段需要预先派生子进程，所以会启动比较慢
 */

#include "lib.h"

static int nchildren;
static pid_t *pids;
static void sig_int(int);

static void
sig_int(int signo){

    int i;

    // terminate all children
    for (i = 0; i < nchildren; i++){
        kill(pids[i], SIGTERM);
    }

    while (wait(NULL) > 0);
    if (errno != ECHILD){
        err_sys("wait error");
    }
    pr_cpu_time();
    exit(0);
}

int
main(int argc, char *argv[]){

    int listenfd, i;
    socklen_t addrlen;

    if (argc == 3){
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 4){
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
    } else{
        err_quit("usage: serv02 [ <host> ] <port#> <#children>");
    }

    nchildren = atoi(argv[argc - 1]);
    pids = Calloc(nchildren, sizeof(pid_t));
    for (i = 0; i < nchildren; i++){
        pids[i] = child_make(i, listenfd, addrlen); // parent return
    }

    Signal(SIGINT, sig_int);
    while (true){
        pause(); // everything done by children
    }
}