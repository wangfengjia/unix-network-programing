//
// Created by 王勇椿 on 2020/12/27.
//
/**
 * 服务器程序(线程池第二版):服务器启动阶段创建一个线程池，并且只让主线程调用accept并把每个客户连接传递给线程池中某个空闲线程
 * 优点:可以解决线程池第一版的内核惊群问题(当一个连接到达时，所有阻塞在accept调用中的线程都会被惊醒)
 */

#include "pthread06.h"

static void sig_int(int);
static int nthreads;
pthread_mutex_t clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clifd_cond = PTHREAD_COND_INITIALIZER;

int
main(int argc, char *argv[]){

    int i, listenfd, connfd;
    socklen_t addrlen, clilen;
    struct sockaddr *cliaddr;

    if (argc == 3){
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 4){
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
    } else{
        err_quit("usage: serv06 [ <host> ] <port#> <#threads>");
    }

    cliaddr = Malloc(addrlen);
    nthreads = atoi(argv[argc - 1]);
    tptr = Calloc(nthreads, sizeof(Thread));
    iget = iput = 0;

    // create all the threads
    for (i = 0; i < nthreads; i++){
        thread_make_v2(i);
    }
    Signal(SIGINT, sig_int);

    while (true){

        clilen = addrlen;
        connfd = Accept(listenfd, cliaddr, &clilen);
        Pthread_mutex_lock(&clifd_mutex);
        clifd[iput] = connfd;
        if (++iput == MAXNCLI){
            iput = 0;
        }
        if (iput == iget){
            err_quit("iput = iget = %d", iput);
        }
        Pthread_cond_signal(&clifd_cond);
        Pthread_mutex_unlock(&clifd_mutex);
    }
}

static void
sig_int(int signo){

    int i;

    pr_cpu_time();
    for (i = 0; i < nthreads; i++){
        printf("thread %d, %ld connections\n", i, tptr[i].thread_count);
    }
    exit(0);
}