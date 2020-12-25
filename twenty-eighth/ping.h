//
// Created by 王勇椿 on 2020/12/24.
//

#ifndef UNIX_NETWORK_PROGRAMING_PING_H
#define UNIX_NETWORK_PROGRAMING_PING_H

/**
 * ping程序:往某个IP地址发送一个ICMP回射请求，该节点则以一个ICMP回射应答响应
 */
#include "../lib/unp.h"
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define BUFFSIZE 1500

char sendbuf[BUFFSIZE];
int datalen; // bytes of data following ICMP header
char *host;
int nsent; //add 1 for each sendto()
pid_t pid;
int sockfd;
int verbose;

void init_v6(void);
void proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void proc_v6(char *, ssize_t, struct msghdr *, struct timeval *);
void send_v4(void);
void send_v6(void);
void readloop(void);
void sig_alrm(int);
void tv_sub(struct timeval *, struct timeval *);

struct proto{
    void (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
    void (*fsend)(void);
    void (*finit)(void);
    struct sockaddr *sasend; //sockaddr{} for send, from getaddrinfo
    struct sockaddr *sarecv; // sockaddr{} for receiving
    socklen_t salen; // length of sockaddr{}s
    int icmpproto; // IPPROTO_xxx value for ICMP
} *pr;

#ifdef IPV6
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#endif
#endif //UNIX_NETWORK_PROGRAMING_PING_H
