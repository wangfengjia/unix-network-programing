//
// Created by 王勇椿 on 2020/12/14.
//

#ifndef UNIX_NETWORK_PROGRAMING_LIB_H
#define UNIX_NETWORK_PROGRAMING_LIB_H

#include "../lib/unp.h"
#include "../lib/unpifi.h"
#include <net/if_dl.h>
#include <sys/param.h>

/**
 * 获得所获取数据报的目的地址(通过IP_RECVDSTADDR套接字选项获取)和所获取数据报接收接口的索引(通过IP_RECVIF套接字选项)
 * @pktp 函数返回时，此指针指向的结构体含有所接收数据报的目的IPv4地址和数据报接收接口的索引
 * @return
 */
ssize_t udp_recvfrom_flags(int, void *, size_t, int *, SA *, socklen_t *, struct unp_in_pktinfo *pktp);
//测试udp_recvfrom_flags函数的方法
void udp_dg_echo(int, SA *, socklen_t);
#endif //UNIX_NETWORK_PROGRAMING_LIB_H
