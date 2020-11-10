//
// Created by 王勇椿 on 2020/11/10.
//
//poll版本的tcp服务器

#include "../lib/unp.h"
#include <limits.h>  //for open max
#include <stddef.h>
#include <stdbool.h>

int
main(int argc, char *argv[]){

    int i, maxi, listenfd, connfd, sockfd;
    int nready;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t  clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < OPEN_MAX; i++){
        client[i].fd = -1;
    }

    maxi = 0; //max index in client[] array
    while (true){

        nready = Poll(client, maxi + 1, INFTIM);

        if (client[0].revents & POLLRDNORM){ //new client connection
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);

            for (i = 1; i < OPEN_MAX; i++){
                if (client[i].fd < 0){
                    client[i].fd = connfd;
                    break;
                }
            }

            if (i == OPEN_MAX){
                err_quit("too many clients");
            }

            client[i].events = POLLRDNORM;
            if (i > maxi){
                maxi = i; //max index in client[] array
            }
            if (--nready <= 0){
                continue; //no more readable descriptors
            }
        }

        //check all client for data
        for (i = 1; i <= maxi; i++){
            if ((sockfd = client[i].fd) < 0){
                continue;
            }
            //检查POLLERR的原因是:有一些实现在一个连接上接收到RST时返回的是POLLERR事件，而其他的实现返回的只是POLLRDNORM事件，无论哪种情形，
            //我们都调用read,当有错误发生时,read将返回这个错误
            if (client[i].revents & (POLLRDNORM | POLLERR)){
                if ((n = read(sockfd, buf, MAXLINE)) < 0){
                    //connection reset by client
                    if (errno == ECONNRESET){
                        Close(sockfd);
                        client[i].fd = -1;
                    } else{
                        err_sys("read error");
                    }
                } else if (n == 0){
                    //connection closed by client
                    Close(sockfd);
                    client[i].fd = -1;
                } else {
                    Writen(sockfd, buf, n);
                }

                if (--nready <= 0){
                    continue;
                }
            }
        }
    }
}