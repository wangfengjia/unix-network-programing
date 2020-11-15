//
// Created by 王勇椿 on 2020/11/14.
//
//调用connect函数,并以read和write调用代替sendto和recvfrom的调用的的客户端

#include "../lib/unp.h"
#include <stddef.h>
#include <stdbool.h>

static void udp_dg_cli(FILE *, int, const SA *, socklen_t);

int
main(int argc, char *argv[]){

    int sockfd;
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);

    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    udp_dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));
    exit(0);
}

static void
udp_dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen){

    int n;
    char sendline[MAXLINE], recvline[MAXLINE];

    Connect(sockfd, (SA *)pservaddr, servlen);
    while (Fgets(sendline, MAXLINE, fp) != NULL){

        Write(sockfd, sendline, strlen(sendline));
        n = Read(sockfd, recvline, MAXLINE);
        recvline[n] = 0; //null terminate
        Fputs(recvline, stdout);
    }
}


