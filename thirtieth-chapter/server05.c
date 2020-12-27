//
// Created by 王勇椿 on 2020/12/27.
//
/**
 * 线程池版本(第一版)的服务器程序:服务器启动的时候预先创建一个线程池，并让每个线程各自调用accept,并且用互斥锁来保证任何时刻只有一个线程在调用accept
 * 优点:比子进程池版本的服务器程序效率更高，由于线程的创建比进程的创建代价小很多
 */

#include "lib.h"

static void sig_int(int);
pthread_mutex_t thread_mlock = PTHREAD_MUTEX_INITIALIZER;

int
main(int argc, char *argv[]){

    int i;

    if (argc == 3){
        thread_listenfd = Tcp_listen(NULL, argv[1], &thread_addrlen);
    } else if (argc == 4){
        thread_listenfd = Tcp_listen(argv[1], argv[2], &thread_addrlen);
    } else{
        err_quit("usage: serv07 [ <host> ] <port#> <#threads>");
    }

    thread_nthreads = atoi(argv[argc - 1]);
    tptr = Calloc(thread_nthreads, sizeof(Thread));

    for (i = 0; i < thread_nthreads; i++){
        thread_make(i); // only main thread returns
    }
    Signal(SIGINT, sig_int);

    while (true){
        pause(); // everything done by threads
    }
}

static void
sig_int(int signo){

    int i;

    pr_cpu_time();
    for (i = 0; i < thread_nthreads; i++){
        printf("thread %d, %ld connections\n", i, tptr[i].thread_count);
    }
    exit(0);
}