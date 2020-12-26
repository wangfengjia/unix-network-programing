//
// Created by 王勇椿 on 2020/12/26.
//
/**
 * 服务器程序使用进程池的第三个版本:只让父进程调用accept，然后把父进程所接受的已连接套接字传递给某个空闲子进程
 * 优点:绕过了为所有子进程调用的accept调用提供上锁保护的可能需求(solaris中需要为多个进程对同一个监听套接字描述符调用accept提供加锁机制)
 * 缺点:代码复杂度比较高，因为父进程必须跟踪子进程的忙闲状态，以便给空闲子进程传递新的已连接描述符
 */

#include "lib.h"

//终止服务器程序前显示各个子进程已处理客户的数量
static void sig_int(int);
static int nchildren;

int
main(int argc, char *argv[]){

    int listenfd, i, navail, maxfd, nsel, connfd, rc;
    ssize_t n;
    fd_set rset, masterset;
    socklen_t addrlen, clilen;
    struct sockaddr *cliaddr;

    if (argc == 3){
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 4){
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
    } else {
        err_quit("usage serv03 [ <host> ] <port#> <#children>");
    }

    FD_ZERO(&masterset);
    FD_SET(listenfd, &masterset);
    maxfd = listenfd;
    cliaddr = Malloc(clilen);

    nchildren = atoi(argv[argc - 1]);
    navail = nchildren;
    cptr = Calloc(nchildren, sizeof(Child));

    for (i = 0; i < nchildren; i++){
        child_make_v2(i, listenfd, addrlen);
        FD_SET(cptr[i].child_pipefd, &masterset);
        maxfd = max(maxfd, cptr[i].child_pipefd);
    }
    Signal(SIGINT, sig_int);

    while (true){
        rset = masterset;
        if (navail <= 0){
            FD_CLR(listenfd, &rset); //turn off if no available children
        }

        nsel = Select(maxfd + 1, &rset, NULL, NULL, NULL);
        //check new connection,当有新的已完成连接(三次握手已完成)时，监听套接字变成可读
        if (FD_ISSET(listenfd, &rset)){
            clilen = addrlen;
            connfd = Accept(listenfd, cliaddr, &clilen);

            //找到空闲的子进程
            for (i = 0; i < nchildren; i++){
                if (cptr[i].child_status == 0){
                    break;
                }
            }
            if (i == nchildren){
                err_quit("no available children");
            }
            cptr[i].child_status = 1; //mark child as busy
            cptr[i].child_count++;
            navail--;

            n = Write_fd(cptr[i].child_pipefd, "", 1, connfd);
            Close(connfd);
            if (--nsel == 0){
                continue; // all done with select() results
            }
        }

        //当子进程完成处理客户请求时，会告知父进程自己空闲
        for (i = 0; i < nchildren; i++){
            if (FD_ISSET(cptr[i].child_pipefd, &rset)){
                if ((n = Read(cptr[i].child_pipefd, &rc, 1)) == 0){
                    err_quit("child %d terminated unexpected", i);
                }
                cptr[i].child_status = 0;
                navail++;
                if (--nsel == 0){
                    break; // all done with select() results
                }
            }
        }
    }
}

static void
sig_int(int signo){

    int i;

    //终止所有子进程
    for (i = 0; i < nchildren; i++){
        kill(cptr[i].child_pid, SIGTERM);
    }

    //wait for all children
    while (wait(NULL) > 0);

    pr_cpu_time();
    for (i = 0; i < nchildren; i++){
        printf("child %d, %ld connections\n", i, cptr[i].child_count);
    }
}

