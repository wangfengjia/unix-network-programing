//
// Created by 王勇椿 on 2020/12/17.
//
//带外数据接收者调用sockatmark来确定何时碰到带外字节。当碰到带外字节时，读入带外字节

#include "../lib/unp.h"
#include <stdbool.h>

int
main(int argc, char *argv[]) {

    int listenfd, connfd, n, on = 1;
    char buff[100];

    if (argc == 2) {
        listenfd = Tcp_listen(NULL, argv[1], NULL);
    } else if (argc == 3) {
        listenfd = Tcp_listen(argv[1], argv[2], NULL);
    } else {
        err_quit("usage: tcprecv04 [ <host> ] <port#>");
    }


    /**
     * 我们希望在线接收带外数据，所以必须开启SO_OOBINLINE套接字选项。但是如果我们等到accept返回之后再在已连接套接字上开启这个选项，
     * 这个时候三路握手已经完成，带外数据也可能已经到达，因此我们必须在监听套接字上开启这个选项，因为所有的套接字选项会从监听套接字
     * 传承给已连接套接字
     */
    Setsockopt(listenfd, SOL_SOCKET, SO_OOBINLINE, &on, sizeof(on));
    connfd = Accept(listenfd, NULL, NULL);

    //sleep的目的:接收来自发送进程的所有数据
    sleep(5);
    while (true) {
        if (Sockatmark(connfd)) {
            printf("at OOB mark\n");
        }
        if ((n = Read(connfd, buff, sizeof(buff) - 1)) == 0) {
            printf("received EOF\n");
            exit(0);
        }

        buff[n] = 0; //null terminate
        printf("read %d bytes: %s\n", n, buff);
    }

}
