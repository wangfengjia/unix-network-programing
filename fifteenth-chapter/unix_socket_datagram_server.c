//
// Created by 王勇椿 on 2020/12/1.
//
//使用Unix域数据报协议的回射服务器(unix域数据报套接字)

#include "../lib/unp.h"

int
main(int argc, char *argv[]){

    int sockfd;
    struct sockaddr_un servaddr, cliaddr;

    sockfd = Socket(AF_LOCAL, SOCK_DGRAM, 0);

    unlink(UNIXDG_PATH);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXDG_PATH);

    Bind(sockfd, (SA *)&servaddr, sizeof(servaddr));
    dg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr));
}

