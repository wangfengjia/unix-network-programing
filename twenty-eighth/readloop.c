//
// Created by 王勇椿 on 2020/12/24.
//
#include "ping.h"
#include <stdbool.h>

void
readloop(void){

    int size;
    char recvbuf[MAXLINE];
    char controlbuf[MAXLINE];
    struct msghdr msg;
    struct iovec iov;
    ssize_t n;
    struct timeval tval;

    sockfd = Socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
    setuid(getuid()); // don't need special permissions any more
    if (pr->finit){
        (*pr->finit)();
    }

    size = 60 * 1024; // Ok if setsockopt fails
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    sig_alrm(SIGALRM); // send first packet

    iov.iov_base = recvbuf;
    iov.iov_len = sizeof(recvbuf);
    msg.msg_name = pr->sarecv;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = controlbuf;

    //读入返回到原始ICMP套接字的每个分组，调用Gettimeofday函数记录分组收取时刻,然后调用合适的协议函数(proc_v4或proc_v6)处理包含在该分组中的ICMP信息
    while (true){

        msg.msg_namelen = pr->salen;
        msg.msg_controllen = sizeof(controlbuf);
        n = recvmsg(sockfd, &msg, 0);
        if (n < 0){
            if (errno == EINTR){
                continue;
            } else{
                err_sys("recvmsg error");
            }
        }

        Gettimeofday(&tval, NULL);
        (*pr->fproc)(recvbuf, n, &msg, &tval);
    }
}

