//
// Created by 王勇椿 on 2020/11/12.
//
//知道客户端临时端口号的进程都可以往客户端发送数据，而且这些数据报会和正常的服务器应答混杂，解决办法是:
//recvfrom调用以返回数据报发送者的IP地址和端口号，保留来自数据报所发往服务器的应答，而忽略任何其他数据报

#include "../lib/unp.h"
#include <stdbool.h>
#include <stddef.h>

static void udp_dg_cli(FILE *, int, const SA *, socklen_t);

int
main(int argc, char *argv[]){

    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2){
        err_quit("usage: udp_cli_v2 <IPaddress>");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(7);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    udp_dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));
    exit(0);
}

static void
udp_dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen){

    int n;
    char sendline[MAXLINE], recvline[MAXLINE];
    socklen_t len;
    struct sockaddr *preply_addr;

    preply_addr = malloc(servlen);
    while (Fgets(sendline, MAXLINE, fp) != NULL){

        Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        len = servlen;
        n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
        if (len != servlen || memcmp(pservaddr, preply_addr, len) != 0){
            printf("reply from %s (ignored)\n", Sock_ntop(preply_addr, len));
            continue;
        }

        recvline[n] = 0; //null terminate
        Fputs(recvline, stdout);
    }
}

