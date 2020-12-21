//
// Created by 王勇椿 on 2020/12/20.
//
//服务器程序心跳函数

#include "heartbeat_lib.h"

static int servfd;
static int nsec; // seconds between each alarm
static int maxnalarms; // w/no client probe before quit
static int nprobes; // since last client probe

static void sig_urg(int);
static void sig_alarm(int);

void heartbeat_serv(int servfd_arg, int nsec_arg, int maxnalarms_arg){


    servfd = servfd_arg; // set globals for signal handlers
    if ((nsec = nsec_arg) < 1){
        nsec = 1;
    }
    if ((maxnalarms = maxnalarms_arg) < nsec){
        maxnalarms = nsec;
    }

    Signal(SIGURG, sig_urg);
    Fcntl(servfd, F_SETOWN, getpid());

    Signal(SIGALRM, sig_alarm);
    alarm(nsec);
}
static void
sig_urg(int signo){

    int n;
    char c;

    if ((n = recv(servfd, &c, 1, MSG_OOB)) < 0){
        if (errno != EWOULDBLOCK){
            err_sys("recv error");
        }
    }

    Send(servfd, &c, 1, MSG_OOB); // echo back out-of-band byte
    nprobes = 0; //reset counter
    return; // may interrupt server code
}

static void
sig_alarm(int signo){

    if (++nprobes > maxnalarms){
        printf("no probes from client\n");
        exit(0);
    }

    alarm(nsec);
    return; // may interrupt server code
}



