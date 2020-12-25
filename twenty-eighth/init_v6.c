//
// Created by 王勇椿 on 2020/12/24.
//
/**
 * 如果用户没有指定-v命令行选项，就在原始ICMPv6套接字上安装一个过滤器，阻止除回射应答外的所有ICMPv6消息，这么做可以缩减这个套接字上收取的分组数
 */
#include "ping.h"

void
init_v6(){

#ifdef IPV6
    int on = 1;

    if (verbose == 0){
        //install a filter that only pass ICMP6_ECHO_REPLY unless verbose
        struct icmp6_filter myfilter;
        ICMP6_FILTER_SETBLOCKALL(&myfilter);
        ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &myfilter);
        setsockopt(sockfd, IPPROTO_IPV6, ICMP6_FILTER, &myfilter, sizeof(myfilter));

        //ignore error return; the filter is optimization
    }

    // ignore error returned below; we just won't receive the hop limit
#ifdef IPV6_RECVHOPLIMIT
    //RFC 3542
    setsockopt(sockfd, IPPROTO_IPV6, IPV6_RECVHOPLIMIT, &on, sizeof(on));
#else
    // RFC 2292
    setsockopt(sockfd, IPPROTO_IPV6, IPV6_2292HOPLIMIT, &on, sizeof(on));
#endif
#endif
}