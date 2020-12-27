//
// Created by 王勇椿 on 2020/12/27.
//
#include "pthread06.h"

static void *thread_main(void *arg);

void
thread_make_v2(int i){

    Pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *)i);
    return; // main thread return
}

static void
*thread_main(void *arg){

    int connfd;

    printf("thread %d starting\n", (int) arg);
    while (true){
        Pthread_mutex_lock(&clifd_mutex);
        //只有一个线程会进入等待的状态，新的连接到来的时候只会唤醒这个线程，不会出现惊群的现象
        while (iget == iput){
            Pthread_cond_wait(&clifd_cond, &clifd_mutex);
        }
        connfd = clifd[iget];
        if (++iget == MAXNCLI){
            iget = 0;
        }
        Pthread_mutex_unlock(&clifd_mutex);
        tptr[(int) arg].thread_count++;

        web_child(connfd);
        Close(connfd);
    }
}

