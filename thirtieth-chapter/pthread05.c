//
// Created by 王勇椿 on 2020/12/27.
//
#include "lib.h"

static void *thread_main(void *);

void
thread_make(int i){

    Pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *)i);
    return; //main thread returns
}

static void *
thread_main(void *arg){

    int connfd;
    socklen_t clilen;
    struct sockaddr *cliaddr;

    cliaddr = Malloc(thread_addrlen);
    printf("thread %d starting\n", (int) arg);

    while (true){
        clilen = thread_addrlen;
        Pthread_mutex_lock(&thread_mlock);
        connfd = Accept(thread_listenfd, cliaddr, &clilen);
        Pthread_mutex_unlock(&thread_mlock);
        tptr[(int) arg].thread_count++;

        web_child(connfd);
        Close(connfd);
    }
}
