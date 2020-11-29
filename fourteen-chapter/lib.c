//
// Created by 王勇椿 on 2020/11/26.
//
#include "lib.h"
#include <stddef.h>
#include <sys/socket.h>
#include <sys/types.h>
static void sig_alarm(int);

int
connect_timeo_signal(int sockfd, const SA *saptr, socklen_t salen, int nsec){

    Sigfunc *sigfunc;
    int n;

    sigfunc = Signal(SIGALRM, sig_alarm);
    if (alarm(nsec) != 0){
        err_msg("connect_timeo_signal: alarm was already set");
    }
    if ((n = connect(sockfd, saptr, salen)) < 0){
        close(sockfd);
        if (errno == EINTR){
            errno = ETIMEDOUT;
        }
    }

    alarm(0); //turn off the alarm
    Signal(SIGALRM, sigfunc); //restore previous signal handler
    return (n);
}

void
dg_cli_signal(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen){

    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    Signal(SIGALRM, sig_alarm);
    while (Fgets(sendline, MAXLINE, fp) != NULL){

        Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        alarm(5);
        if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0){
            //被信号处理函数中断，输出一个信息并继续执行
            if (errno == EINTR){
                fprintf(stderr, "socket timeout\n");
            } else{
                err_sys("recvfrom error");
            }
        } else{
            //读到一行来自服务器的文本，那就关闭报警时钟并输出服务器的应答
            alarm(0);
            recvline[n] = 0; // null terminate
            Fputs(recvline, stdout);
        }
    }
}

void
dg_cli_select(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen){

    int n;
    char sendline[MAXLINE], recvline[MAXLINE];

    while (Fgets(sendline, MAXLINE, fp) != NULL){

        Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        //直到readable_timeo_select函数告知所关注的描述符已经变为可读后我们才调用recvfrom,这一点保证recvfrom调用不会阻塞
        if (readable_timeo_select(sockfd, 5) == 0){
            fprintf(stderr, "socket timeout\n");
        } else{
            n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
            recvline[n] = 0; // null terminate
            Fputs(recvline, stdout);
        }
    }
}

void
dg_cli_sopt(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen){

    int n;
    char sendline[MAXLINE], recvline[MAXLINE];
    struct timeval tv;

    tv.tv_sec = 5;
    tv.tv_usec = 0;
    Setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (Fgets(sendline, MAXLINE, fp) != NULL){

        Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
        if (n < 0){
            if (errno == EWOULDBLOCK){
                fprintf(stderr, "socket timeout\n");
                continue;
            } else{
                err_sys("recvfrom error");
            }
        }

        recvline[n] = 0; // null terminate
        Fputs(recvline, stdout);
    }
}

int
readable_timeo_select(int fd, int sec){

    fd_set rset;
    struct timeval tv;

    FD_ZERO(&rset);
    FD_SET(fd, &rset);

    tv.tv_sec = sec;
    tv.tv_usec = 0;

    return (select(fd + 1, &rset, NULL, NULL, &tv));
}

static void
sig_alarm(int signo){

    return; //just interrupt the connect()
}


