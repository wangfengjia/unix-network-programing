//
// Created by 王勇椿 on 2020/12/7.
//
//输出主机的硬件地址:使用我们的get_ifi_info函数返回一个主机的所有IP地址，然后对每个IP地址发出一个SIOCGARP请求以获取并显示它的硬件地址
#include "../lib/unpifi.h"
#include <net/if_arp.h>

int
main(int argc, char *argv[]){

    int sockfd;
    struct ifi_info *ifi;
    unsigned char *ptr;
    struct arpreq arpreq;
    struct sockaddr_in *sin;

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    for (ifi = get_ifi_info(AF_INET, 0); ifi != NULL; ifi = ifi->ifi_next){
        printf("%s: ", Sock_ntop(ifi->ifi_addr, sizeof(struct sockaddr_in)));

        sin = (struct sockaddr_in *)&arpreq.arp_pa;
        memcpy(sin, ifi->ifi_addr, sizeof(struct sockaddr_in));
        //ioctl SIOCGARP: No such device
        if (ioctl(sockfd, SIOCGARP, &arpreq) < 0){
            err_ret("ioctl SIOCGARP");
            continue;
        }

        ptr = &arpreq.arp_ha.sa_data[0];
        printf("%x:%x:%x:%x:%x:%x\n", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3), *(ptr + 4), *(ptr + 5));
    }

    exit(0);
}
