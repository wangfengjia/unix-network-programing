//
// Created by 王勇椿 on 2020/12/15.
//
//绑定所有地址的UDP服务器程序

#include "../lib/unpifi.h"
#include <stdbool.h>

void mydg_echo(int, SA *, socklen_t, SA *);

int
main(int argc, char *argv[]){

    int sockfd;
    const int on = 1;
    pid_t pid;
    struct ifi_info *ifi, ifihead;
    struct sockaddr_in *sa, cliaddr, wildaddr;

    for (ifihead = ifi = Get_ifi_info(AF_INET, 1); ifi != NULL; ifi = ifi->ifi_next){

        // bind unicast address
        sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
        //设置SO_REUSEADDR选项的目的:我们要给所有IP地址绑定同一个端口
        Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        sa = (struct sockaddr_in *)ifi->ifi_addr;
        sa->sin_family = AF_INET;
        sa->sin_port = htons(SERV_PORT);
        //给UDP套接字绑定捆绑单播地址
        Bind(sockfd, (SA *)sa, sizeof(*sa));
        printf("bound %s\n", Sock_ntop((SA *)sa, sizeof(*sa)));

        if ((pid = fork()) == 0){ // child process
            mydg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr), (SA *)sa);
            exit(0); //never executed
        }

        //如果当前接口支持广播，创建一个UDP套接字并给它捆绑广播地址
        if (ifi->ifi_flags & IFF_BROADCAST){
            sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
            Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
            sa = (struct sockaddr_in *)ifi->ifi_brdaddr;
            sa->sin_family = AF_INET;
            sa->sin_port = htons(SERV_PORT);
            if (bind(sockfd, (SA *)sa, sizeof(*sa)) < 0){
                if (errno == EADDRINUSE){
                    printf("EADDRINUSE: %s\n", Sock_ntop((SA *)sa, sizeof(*sa)));
                    Close(sockfd);
                    continue;
                } else{
                    err_sys("bind error for %s", Sock_ntop((SA *)sa, sizeof(*sa)));
                }
            }

            printf("bound %s\n", Sock_ntop((SA *)sa, sizeof(*sa)));
            if ((pid = fork()) == 0){
                mydg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr), (SA *)sa);
                exit(0);
            }
        }
    }

    //创建UDP套接字，给UDP套接字绑定通配地址
    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bzero(&wildaddr, sizeof(wildaddr));
    wildaddr.sin_family = AF_INET;
    wildaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    wildaddr.sin_port = htons(SERV_PORT);
    Bind(sockfd, (SA *)&wildaddr, sizeof(wildaddr));
    printf("bound %s\n", Sock_ntop((SA *)&wildaddr, sizeof(wildaddr)));

    if ((pid = fork()) == 0){
        mydg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr), (SA *)sa);
        exit(0);
    }

    exit(0);
}


void
mydg_echo(int sockfd, SA *pcliaddr, socklen_t clilen, SA *myaddr){

    int n;
    char mesg[MAXLINE];
    socklen_t len;

    while (true){

        len = clilen;
        n = Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        printf("child %d, datagram from %s", getpid(), Sock_ntop(pcliaddr, len));
        printf(", to %s\n", Sock_ntop(myaddr, clilen));
        Sendto(sockfd, mesg, n, 0, pcliaddr, len);
    }
}
