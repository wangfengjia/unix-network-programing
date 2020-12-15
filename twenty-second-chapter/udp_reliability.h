//
// Created by 王勇椿 on 2020/12/14.
//

#ifndef UNIX_NETWORK_PROGRAMING_UDP_RELIABILITY_H
#define UNIX_NETWORK_PROGRAMING_UDP_RELIABILITY_H

#include "../lib/unp.h"
#include "../lib/unprtt.h"
//给不可靠的数据报(UDP)服务增加可靠性

extern int rtt_debug_flag;

void udp_dg_cli(FILE *, int, const SA *, socklen_t);
ssize_t dg_send_recv(int, const void *, size_t, void *, size_t, const SA *, socklen_t);

void udp_rtt_init(struct rtt_info *);
uint32_t udp_rtt_ts(struct rtt_info *);
void udp_rtt_newpack(struct rtt_info *);
int udp_rtt_start(struct rtt_info *);
void udp_rtt_stop(struct rtt_info *, uint32_t);
int udp_rtt_timeout(struct rtt_info *);

#endif //UNIX_NETWORK_PROGRAMING_UDP_RELIABILITY_H
