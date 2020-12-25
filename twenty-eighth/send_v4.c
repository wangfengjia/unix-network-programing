//
// Created by 王勇椿 on 2020/12/24.
//
//构造一个ICMPv4回射请求消息并把这个消息写到原始套接字

#include "ping.h"

void
send_v4(void){

    int len;
    struct icmp *icmp;

    icmp = (struct icmp *)sendbuf;
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_pid = pid;
    icmp->icmp_seq = nsent++;
    memset(icmp->icmp_data, 0xa5, datalen); // fill with pattern
    Gettimeofday((struct timeval *) icmp->icmp_data, NULL);

    len = 8 + datalen; //checksum ICMP header and data
    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = in_cksum((u_short *) icmp, len);

    Sendto(sockfd, sendbuf, len, 0, pr->sasend, pr->salen);
}