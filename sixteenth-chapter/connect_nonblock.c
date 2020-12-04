//
// Created by 王勇椿 on 2020/12/3.
//
//非阻塞的connect

#include "lib.h"

int
connect_nonblock(int sockfd, const SA *saptr, socklen_t salen, int nsec){

    int flags, n, error;
    socklen_t len;
    fd_set rset, wset;
    struct timeval tval;

    flags = Fcntl(sockfd, F_GETFL, 0);
    Fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    error = 0;
    if ((n = connect(sockfd, saptr, salen)) < 0){
        if (errno != EINPROGRESS){
            return (-1);
        }
    }

    //Do whatever we want while the connect is taking place
    if (n == 0){
        goto done;
    }
    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;
    tval.tv_sec = nsec;
    tval.tv_usec = 0;
    if ((n = Select(sockfd + 1, &rset, &wset, NULL, nsec ? &tval : NULL)) == 0){
        //超时，关闭套接字的目的是防止已经启动的三路握手继续下去
        close(sockfd);
        errno = ETIMEDOUT;
        return (-1);
    }

    //当一个TCP套接字上发生某个错误时，这个待处理错误会导致这个套接字变为即可读又可写
    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)){
        len = sizeof(error);
        //判断套接字上是否有错误
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0){
            return (-1);
        }
    } else{
        err_quit("Select error: sockfd not set");
    }


    //当客户和服务器在同一个主机上时，连接会马上建立
done:
    Fcntl(sockfd, F_SETFL, flags); // restore file status flags
    if (error){
        close(sockfd);
        errno = error;
        return (-1);
    }

    return 0;
}

