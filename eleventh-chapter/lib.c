//
// Created by 王勇椿 on 2020/11/21.
//
#include "../lib/unp.h"
#include "lib.h"
#include <stddef.h>

int
ltcp_connect(const char *host, const char *serv){

    int sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0){
        err_quit("tcp_connect error for %s, %s: %s", host, serv, gai_strerror(n));
    }
    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0){
            continue; // ignore this one address
        }
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0){
            break; // success
        }

        Close(sockfd); // ignore this one
    }while ((res = res->ai_next) != NULL);

    //errno set from final connect
    if (res == NULL){
        err_sys("tcp_connect error for %s, %s", host, serv);
    }
    freeaddrinfo(ressave);
    return (sockfd);
}

int
ltcp_listen(const char *host, const char *serv, socklen_t *addrlenp){

    int listenfd, n;
    const int on = 1;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0){
        err_quit("tcp_listen error for %s, %s: %s", host, serv, gai_strerror(n));
    }

    ressave = res;
    do {

        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (listenfd < 0){
            continue; //error, try next one
        }

        Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0){
            break; //success
        }
        Close(listenfd); // bind error, close and try next one
    }while ((res = res->ai_next) != NULL);

    //errno from final socket() or bind()
    if (res == NULL){
        err_sys("tcp_listen error for %s, %s", host, serv);
    }
    Listen(listenfd, LISTENQ);

    if (addrlenp){
        *addrlenp = res->ai_addrlen; // return size of protocol address
    }
    freeaddrinfo(ressave);

    return (listenfd);
}



int
ludp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp){

    int sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0){
        err_quit("udp_client error for %s, %s: %s", host, serv, gai_strerror(n));
    }

    ressave = res;
    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd >= 0){
            break; //success
        }
    }while ((res = res->ai_next) != NULL);

    // errno set from final socket()
    if (res == NULL){
        err_sys("udp_client error for %s, %s", host, serv);
    }
    *saptr = Malloc(res->ai_addrlen);
    memcpy(*saptr, res->ai_addr, res->ai_addrlen);
    *lenp = res->ai_addrlen;

    freeaddrinfo(ressave);
    return (sockfd);
}

int
ludp_connect(const char *host, const char *serv){

    int sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0){
        err_quit("udp_connect error for %s, %s: %s", host, serv, gai_strerror(n));
    }

    ressave = res;
    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0){
            continue;
        }
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0){
            break; //success
        }
        Close(sockfd);
    }while ((res = res->ai_next) != NULL);

    if (res == NULL){
        err_sys("udp_connect error for %s, %s", host, serv);
    }
    freeaddrinfo(ressave);
    return (sockfd);
}

int
ludp_server(const char *host, const char *serv, socklen_t *addrlenp){

    int sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0){
        err_quit("udp_server error for %s, %s: %s", host, serv, gai_strerror(n));
    }

    ressave = res;
    do {

        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0){
            continue;
        }
        if (bind(sockfd, res->ai_addr, res->ai_addrlen) == 0){
            break; // success
        }

        Close(sockfd); // bind error - close and try one
    }while ((res = res->ai_next) != NULL);

    // errno from final socket() or bind()
    if (res == NULL){
        err_sys("udp_server error for %s, %s", host, serv);
    }
    if (addrlenp){
        *addrlenp = res->ai_addrlen; //size of protocol address
    }

    freeaddrinfo(ressave);
    return (sockfd);
}

