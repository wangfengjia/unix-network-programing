//
// Created by 王勇椿 on 2020/12/20.
//
/**
 * 客户端心跳函数。
 * 方法:客户每隔一秒向服务器发送一个带外字节，服务器收到这个带外字节后向客户发送回一个带外字节。客户和服务器每隔一秒钟递增它们的cnt变量一次，
 * 每收到一个带外字节又把这个变量置为0，如果这个计数器达到5(也就说本进程已有5秒钟没有收到来自对端的带外字节)，就认定连接失效。当有带外字节到达时，
 * 客户和服务器都使用SIGURG信号得以通知。正常数据和带外字节都通过单个TCP连接交换
 */

#include "heartbeat_lib.h"

static int servfd;
static int nsec; //seconds between each alarm
static int maxnprobes; // probes w/no response before quit
static int nprobes; // probes since last server response

static void sig_urg(int);
static void sig_alarm(int);

void
myheartbeat_cli(int servfd_arg, int nsec_arg, int maxnprobes_arg){

    servfd = servfd_arg; // set globals for signal handlers
    if ((nsec = nsec_arg) < 1){
        nsec = 1;
    }
    if ((maxnprobes = maxnprobes_arg) < nsec){
        maxnprobes = nsec;
    }
    nprobes = 0;

    Signal(SIGURG, sig_urg);
    Fcntl(servfd, F_SETOWN, getpid());

    Signal(SIGALRM, sig_alarm);
    alarm(nsec);
}

static void
sig_urg(int signo){

    int n;
    char c;

    if ((n = recv(servfd, &c, 1, MSG_OOB)) < 0){
        if (errno != EWOULDBLOCK){
            err_sys("recv error");
        }
    }

    nprobes = 0; //reset counter
    return; //may interrupt client code
}

static void
sig_alarm(int signo){

    if (++nprobes > maxnprobes){
        fprintf(stderr, "server is unreachable \n");
        exit(0);
    }

    Send(servfd, "1", 1, MSG_OOB);
    alarm(nsec);
    return; // may interrupt client code
}