//
// Created by 王勇椿 on 2020/12/26.
//
#include "lib.h"
#include <stdbool.h>

static void child_main(int, int, int);

static void
child_main(int i, int listenfd, int addrlen){

    char c;
    int connfd;
    ssize_t n;

    printf("child %ld starting\n", (long)getpid());
    while (true){
        if ((n = Read_fd(STDERR_FILENO, &c, 1, &connfd)) == 0){
            err_quit("read_fd returned");
        }
        if (connfd < 0){
            err_quit("no descriptor from read_fd");
        }

        web_child(connfd); //process request
        Close(connfd);

        Write(STDERR_FILENO, "", 1); //tell parent we're ready again
    }
}

pid_t
child_make_v2(int i, int listenfd, int addrlen){

    int sockfd[2];
    pid_t pid;

    Socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);
    if ((pid = fork()) > 0){ // parent process
        Close(sockfd[1]);
        cptr[i].child_pid = pid;
        cptr[i].child_pipefd = sockfd[0];
        cptr[i].child_status = 0;
        return (pid);
    }

    //child's stream pipe to parent
    Dup2(sockfd[1], STDERR_FILENO);
    Close(sockfd[0]);
    Close(sockfd[1]);
    Close(listenfd); // child don't need this open
    child_main(i, listenfd, addrlen); // never return
}

