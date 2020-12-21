//
// Created by 王勇椿 on 2020/12/17.
//
/**
 * 展示带外数据的两个特性
 * 1.即使因为流量控制而停止发送数据了，TCP还是会发送带外数据的通知(紧急指针)
 * 2.在带外数据到达之前，接收进程可能被通知说发送进程已经发送了带外数据(使用SIGURG信号或通过select)。如果接收进程接着指定MSG_OOB调用recv,
 *  而带外数据却尚未到达，recv将返回EWOULDBLOC错误
 */

#include "../lib/unp.h"

int
main(int argc, char *argv[]){

    int sockfd, size;
    char buff[16384];

    if (argc != 3){
        err_quit("usage: tcpsend05 <host> <port#>");
    }

    sockfd = Tcp_connect(argv[1], argv[2]);
    size = 32768;
    Setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));

    Write(sockfd, buff, 16384);
    printf("wrote 16384 bytes of normal data\n");
    sleep(5);

    Send(sockfd, "a", 1, MSG_OOB);
    printf("wrote 1 byte of OOB data");

    Write(sockfd, buff, 1024);
    printf("wrote 1024 bytes of normal data\n");

    exit(0);
}
