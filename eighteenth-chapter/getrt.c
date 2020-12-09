//
// Created by 王勇椿 on 2020/12/7.
//
//通过路由套接字发出RTM_GET命令(内核在它的和参数对应的路由表中查找参数地址，并作为一个RTM_GET信息返回相应路由表项的信息)

#include "../libroute/unproute.h"
#include "../lib/unp.h"
#include <sys/socket.h>

static
#define BUFLEN (sizeof(struct rt_msghdr) + 512)
#define SEQ 9999

int
main(int argc, char *argv[]){

    int sockfd;
    char *buf;
    pid_t pid;
    ssize_t n;
    struct rt_msghdr *rtm;
    struct sockaddr *sa, *rti_info[RTAX_MAX];
    struct sockaddr_in *sin;

    if (argc != 2){
        err_quit("usage: getrt <IPaddress>");
    }

    //需要超级用户权限
    sockfd = Socket(AF_ROUTE, SOCK_RAW, 0);
    buf = Calloc(1, BUFLEN);

    rtm = (struct rt_msghdr *)buf;
    rtm->rtm_msglen = sizeof(struct rt_msghdr) + sizeof(struct sockaddr_in);
    rtm->rtm_version = RTM_VERSION;
    rtm->rtm_type = RTM_GET;
    rtm->rtm_addrs = RTA_DST;
    rtm->rtm_pid = pid = getpid();
    rtm->rtm_seq = SEQ;

    sin = (struct sockaddr_in *) (rtm + 1);
    sin->sin_len = sizeof(struct sockaddr_in);
    sin->sin_family = AF_INET;
    Inet_pton(AF_INET, argv[1], &sin->sin_addr);

    Write(sockfd, rtm, rtm->rtm_msglen);
    do {
        n = Read(sockfd, rtm, BUFLEN);
        /**
         * 其他进程也可能打开路由套接字，而且内核给所有路由套接字都传送一个全部路由消息的副本，我们就需要检查消息的类型、序列号和进程ID
         * 以确保收到的消息正是我们所等待的应答
         */
    } while (rtm->rtm_type != RTM_GET || rtm->rtm_seq != SEQ || rtm->rtm_pid != pid);
    rtm = (struct rt_msghdr *)buf;
    sa = (struct sockaddr *)(rtm + 1);
    get_rtaddrs(rtm->rtm_addrs, sa, rti_info);

    if ((sa = rti_info[RTA_DST]) != NULL){
        printf("dest: %s\n", Sock_ntop_host(sa, sa->sa_len));
    }
    if ((sa = rti_info[RTAX_GATEWAY]) != NULL){
        printf("gateway: %s\n", Sock_ntop_host(sa, sa->sa_len));
    }
    if ((sa = rti_info[RTAX_NETMASK]) != NULL){
        printf("netmask: %s\n", Sock_masktop(sa, sa->sa_len));
    }
    if ((sa = rti_info[RTAX_GENMASK]) != NULL){
        printf("genmask: %s\n", Sock_masktop(sa, sa->sa_len));
    }
}


