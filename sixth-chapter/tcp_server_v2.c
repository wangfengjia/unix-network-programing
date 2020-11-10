//
// Created by 王勇椿 on 2020/11/9.
//
//select版本的server,用一个进程来处理任意个客户端请求，而不用为每个客户派生一个进程
#include "../lib/unp.h"
#include <sys/select.h>
#include <stddef.h>
#include <stdbool.h>

int
main(int argc, char *argv[]){

    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    maxfd = listenfd; // initialize
    maxi = -1; //index into client[] array
    for (i = 0; i < FD_SETSIZE; i++){
        client[i] = -1; // -1 indicates available index
    }
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);


    while (true){
        rset = allset; //structure assignment
        //利用select函数的返回值来减少检查套接字描述符的数量
        nready = Select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(listenfd, &rset)){ //new client connection
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);

            for (i = 0; i < FD_SETSIZE; i++){
                if (client[i] < 0){
                    client[i] = connfd; //save descriptor
                    break;
                }
            }
            if (i == FD_SETSIZE){
                err_quit("too many clients");
            }
            FD_SET(connfd, &allset); // add new descriptor to set
            if (connfd > maxfd){
                maxfd = connfd;
            }
            if (i > maxi){
                maxi = i; // max index in client[] array
            }
            if (--nready <= 0){
                continue; //no more readable descriptors
            }
        }

        //check all clients for data
        for (i = 0; i <= maxi; i++){
            if ((sockfd = client[i]) < 0){
                continue;
            }
            if (FD_ISSET(sockfd, &rset)){
                if ((n = Read(sockfd, buf, MAXLINE)) == 0){
                    //connection closed by client
                    close(sockfd);
                    FD_CLR(sockfd, &rset);
                    client[i] = -1;
                } else{
                    Writen(sockfd, buf, n);
                }

                //no more readable descriptors
                if (--nready <= 0){
                    break;
                }
            }
        }
    }
}
