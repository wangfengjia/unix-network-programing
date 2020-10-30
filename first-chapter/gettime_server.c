//
// Created by 王勇椿 on 2020/10/29.
//
//例子:当前时间查询服务端程序

#include "../lib/unp.h"
#include <time.h>
#include <stdbool.h>
#include <stddef.h>

int
main(int argc, char *argv[]){

    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
    time_t ticks;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    //把套接字转换成监听套接字，这样客户端的连接请求就可以在这个监听套接字上由内核接受
    Listen(listenfd, LISTENQ);
    while (true){
        //调用Accept函数后，进程会进入阻塞状态; Accept函数返回tiaojian:一个客户端的连接请求到来并被内核接受(TCP连接使用三次握手来建立连接，握手完毕)
        //Accept函数的返回值connfd是已连接描述符，是一个新的套接字描述符，与原始套接字描述符有相同的类型。connfd套接字描述符会与客户端相连接，Accept函数中的原始套接字描述符
        //不会关联到这个连接
        connfd = Accept(listenfd, (SA *)NULL, NULL);
        ticks = time(NULL);
        snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buf, strlen(buf));
        close(connfd);
    }
}

