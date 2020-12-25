//
// Created by 王勇椿 on 2020/12/24.
//
#include "ping.h"

void
proc_v6(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv){

#ifdef IPV6

    double rtt;
    struct icmp6_hdr *icmp6;
    struct timeval *tvsend;
    struct cmsghdr *cmsg;
    int hlim;

    //从原始ICMPv6套接字接收的仅仅是ICMP首部，没有IPv6首部。不同于从IPv4原始套接字递送给进程的包括IPv4首部
    icmp6 = (struct icmp6_hdr *)ptr;
    if (len < 8){
        return; // malformed packet
    }
    if (icmp6->icmp6_type == ICMP6_ECHO_REPLY){
        if (icmp6->icmp6_pid != pid){
            return;
        }
        if (len < 16){
            return; // not enough data to use
        }
        tvsend = (struct timeval *) (icmp6 + 1);
        tv_sub(tvrecv, tvsend);
        rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;

        hlim = -1;
        for (cmsg = CMSG_FIRSTHDR(msg); cmsg != NULL; cmsg = CMSG_NXTHDR(msg, cmsg)){
            if (cmsg->cmsg_level == IPPROTO_IPV6 && cmsg->cmsg_type == IPV6_2292HOPLIMIT){
                hlim = *(u_int32_t *)CMSG_DATA(cmsg);
                break;
            }
        }
        printf("%d bytes from %s: seq=%u, hlim=", len, Sock_ntop_host(pr->sarecv, pr->salen), icmp6->icmp6_seq);
        if (hlim == -1){
            printf("???"); //ancillary data missing
        } else{
            printf("%d", hlim);
        }
        printf(", rtt=%.3f ms\n", rtt);
    } else if (verbose){
        printf(" %d bytes from %s: type = %d, code = %d\n", len, Sock_ntop_host(pr->sarecv, pr->salen), icmp6->icmp6_type, icmp6->icmp6_code);
    }
#endif
}

