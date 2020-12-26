//
// Created by 王勇椿 on 2020/12/25.
//
/**
 * traceroute允许我们确定IP数据报从本地主机到某个远程主机所经过的路径
 * 原理:它使用IPv4的TTL字段或IPv6的调限字段以及两种ICMP消息。它一开始向目的地发送一个TTL(或跳限)为1的UDP数据报，这个数据报导致第一跳路由器返送一个ICMP
 * "time exceeded in transmit"(传输中超时)错误。接着每递增TTL一次发送一个UDP数据报，从而逐步确定下一跳路由器。当某个UDP数据报到达最终目的地，目标是由
 * 这个主机返回一个ICMP "port unreachable(端口不可达)"错误。这个目标通过向一个随机选取的(但愿)未被目的主机使用的端口发送UDP数据报得以实现
 */
#ifndef UNIX_NETWORK_PROGRAMING_TRACEROUTE_H
#define UNIX_NETWORK_PROGRAMING_TRACEROUTE_H

#include "../lib/unp.h"
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>

#define BUFFSIZE 1500

//format of outgoing UDP data
struct rec{
    u_short rec_seq; // sequence number
    u_short rec_ttl; // TTL packet left with
    struct timeval rec_tv; //time packet left
};

char recvbuf[MAXLINE];
char sendbuf[MAXLINE];

int datalen; //bytes of data following ICMP header
char *host;
u_short sport, dport;
int nsent; //add 1 for each sendto()
pid_t pid;
int probe, nprobes;
int sendfd, recvfd; // send on UDP sock, read on raw ICMP sock
int ttl, max_ttl;
int verbose;

const char *icmpcode_v4(int);
const char *icmpcode_v6(int);
int recv_v4(int, struct timeval *);
int recv_v6(int, struct timeval *);
void sig_alrm(int);
void traceloop(void);
void tv_sub(struct timeval *, struct timeval *);

struct proto{

    const char *(*icmpcode)(int);
    int (*recv)(int, struct timeval *);
    struct sockaddr *sasend; //sockaddr{} for send, from getaddrinfo
    struct sockaddr *sarecv; // sockaddr{} for receiving
    struct sockaddr *salst; // last sockaddr{} for receiving
    struct sockaddr *sabind; // sockaddr{} for binding source port
    socklen_t salen; //length of sockaddr{}s
    int icmpproto; //IPPROTO_XXX value for ICMP
    int ttllevel; // setsockopt() level to set TTL
    int ttloptname; // setsockopt() name to set TTL
};

#ifdef IPV6
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#endif

#endif //UNIX_NETWORK_PROGRAMING_TRACEROUTE_H
