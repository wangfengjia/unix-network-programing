//
// Created by 王勇椿 on 2020/12/16.
//
/**
 * (不正确地)使用select得到TCP带外数据通知的接收程序
 * 不正确的地方:select一直指示一个异常条件，直到进程的读入越过带外数据。同一个带外数据不能读入多次，因为首次读入之后，内核就清空这个单字节的缓冲区。
 * 再次指定MSG_OOB标志调用recv时，它就返回EINVAL
 * 解决办法:读入普通数据后才select异常条件
 */


#include "../lib/unp.h"
#include <stdbool.h>

int
main(int argc, char *argv[]){

    int listenfd, connfd;
    char buf[100];
    fd_set rset, xset;

    if (argc == 2){
        listenfd = Tcp_connect(NULL, argv[1], NULL);
    } else if (argc == 3){
        listenfd = Tcp_connect(argv[1], argv[2], NULL);
    } else{
        err_quit("usage: tcprecv02 [ <host> ] <port#>");
    }

    connfd = Accept(listenfd, NULL, NULL);
    FD_ZERO(&rset);
    FD_ZERO(&xset);
    while (true){

        FD_SET(connfd, &rset);
        FD_SET(connfd, &xset);

        Select(connfd + 1, &rset, NULL, &xset, NULL);
        if (FD_ISSET(connfd, &xset)){
            n = Recv(connfd, buf, sizeof(buf) - 1, MSG_OOB);
            buf[n] = 0; //null terminated
            printf("read %d OOB byte: %s\n", n, buf);
        }

        if (FD_ISSET(connfd, &rset)){
            if ((n = Read(connfd, buf, sizeof(buf) - 1)) == 0){
                printf("received EOF\n");
                exit(0);
            }
            buf[n] = 0; //null terminated
            printf("read %d bytes: %s\n", n, buf);
        }
    }
}

