//
// Created by 王勇椿 on 2020/11/4.
//
#include "inet_lib.h"

ssize_t
socket_writen(int fd, const void *vptr, size_t n){

    size_t nleft;
    ssize_t nwriten;
    const char *ptr;

    ptr = vptr;
    nleft = n;

    while (nleft > 0){

        if ((nwriten = write(fd, ptr, nleft)) < 0){
            if (nwriten < 0 && errno == EINTR){
                nwriten = 0; //call write() again
            } else{
                return (-1);
            }
        }
        nleft -= nwriten;
        ptr += nwriten;
    }

    return (n);
}

