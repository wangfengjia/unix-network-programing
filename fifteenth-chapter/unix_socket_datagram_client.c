//
// Created by 王勇椿 on 2020/12/1.
//
//使用unix域数据报协议的回射客户端

#include "../lib/unp.h"
#include <stddef.h>

int
main(int argc, char *argv[]){

    int sockfd;
    struct sockaddr_un cliaddr, servaddr;

    sockfd = Socket(AF_LOCAL, SOCK_DGRAM, 0);
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sun_family = AF_LOCAL;
    strcpy(cliaddr.sun_path, tmpnam(NULL));

    /**
     * 给套接字显式绑定一个路径名的原因是:这样服务器才会有能回答应射的路径名
     */
    Bind(sockfd, (SA *)&cliaddr, sizeof(cliaddr));

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXDG_PATH);

    dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));
    exit(0);
}

