//
// Created by 王勇椿 on 2020/11/22.
//

#include "../lib/unp.h"
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

int
main(int argc, char *argv[]){

    int sockfd;
    ssize_t n;
    char buf[MAXLINE];
    time_t ticks;
    socklen_t len;
    struct sockaddr_storage cliaddr;

    if (argc == 2){
        sockfd = Udp_server(NULL, argv[1], NULL);
    } else if (argc == 3){
        sockfd = Udp_server(argv[1], argv[2], NULL);
    } else{
        err_quit("usage: daytimeudpserv [ <host> ] <service or port>");
    }

    while (true){
        len = sizeof(cliaddr);
        n = Recvfrom(sockfd, buf, MAXLINE, 0, (SA *)&cliaddr, &len);
        printf("datagram from %s\n", Sock_ntop((SA *)&cliaddr, len));

        ticks = time(NULL);
        snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
        Sendto(sockfd, buf, strlen(buf), 0, (SA *)&cliaddr, len);
    }
}
