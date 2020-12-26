//
// Created by 王勇椿 on 2020/12/26.
//
#include "lib.h"



int
main(int argc, char *argv[]){

    int i, j, fd, nchildren, nloops, nbytes;
    pid_t pid;
    ssize_t n;
    char request[MAXLINE], reply[MAXN];

    if (argc != 6){
        err_quit("usage: client <hostname or IPaddr> <port> <#children> <#loops/child> <#bytes/request>");
    }

    nchildren = atoi(argv[3]);
    nloops = atoi(argv[4]);
    nbytes = atoi(argv[5]);
    snprintf(request, sizeof(request), "%d\n", nbytes); // newline at end

    for (i = 0; i < nchildren; i++){
        //child process
        if ((pid = fork()) == 0){
            for (j = 0; j < nloops; j++){
                fd = Tcp_connect(argv[1], argv[2]);
                Write(fd, request, strlen(request));
                if ((n = Readn(fd, reply, nbytes)) != nbytes){
                    err_quit("server returned %d bytes", n);
                }
                Close(fd); // TIME_WAIT on client, not on server
            }
            printf("child %d done", i);
            exit(0);
        }
    }

    //parent wait for all children
    while (wait(NULL) > 0);
    if (errno != ECHILD){
        err_sys("wait error");
    }

    exit(0);
}
