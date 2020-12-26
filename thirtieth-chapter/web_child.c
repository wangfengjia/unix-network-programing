//
// Created by 王勇椿 on 2020/12/26.
//
//处理每一个客户请求

#include "lib.h"

void
web_child(int sockfd){

    int ntowrite;
    ssize_t nread;
    char line[MAXLINE], result[MAXN];

    while (true){
        if ((nread = Readline(sockfd, line, MAXLINE)) == 0){
            return; // connection closed by other end
        }

        //line from client sepcifies #bytes to write back
        ntowrite = atol(line);
        if ((ntowrite <= 0) || (ntowrite > MAXN)){
            err_quit("client request for %d bytes", ntowrite);
        }

        Writen(sockfd, result, ntowrite);
    }
}

