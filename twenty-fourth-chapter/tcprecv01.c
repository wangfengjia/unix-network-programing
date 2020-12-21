//
// Created by 王勇椿 on 2020/12/16.
//
//TCP带外数据接收程序

#include "../lib/unp.h"
#include <stdbool.h>

int listenfd, connfd;

/**
 * SIGURG信号的处理函数
 * 当有新的紧急指针(不论由这个指针指向的实际数据字节是否已经到达接收端TCP)到达时，
 * 内核给接收套接字的属主(通过调用fcntl或ioctl为套接字建立属主)进程发送SIGURG信号。
 *
 * @param signo
 */
void sig_urg(int signo);


int
main(int argc, char *argv[]){

    int n;
    char buff[100];

    if (argc == 2){
        listenfd = Tcp_listen(NULL, argv[1], NULL);
    } else if (argc == 3){
        listenfd = Tcp_listen(argv[1], argv[2], NULL);
    } else{
        err_quit("usage: tcprecv01 [ <host> ] <port#>");
    }

    connfd = Accept(listenfd, NULL, NULL);
    Signal(SIGURG, sig_urg);
    Fcntl(connfd, F_SETOWN, getpid());

    while (true){
        if ((n = Read(connfd, buff, sizeof(buff) - 1)) == 0){
            printf("received EOF\n");
            exit(0);
        }

        buff[n] = 0; //null terminated
        printf("read %d bytes: %s\n", n, buff);
    }
}


void
sig_urg(int signo){

    int n;
    char buf[100];

    printf("SIGURG received\n");
    n = Recv(connfd, buf, sizeof(buf) - 1, MSG_OOB);
    buf[n] = 0; //null terminated
    printf("read %d OOB bytes: %s\n", n, buf);
}