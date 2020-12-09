//
// Created by 王勇椿 on 2020/12/8.
//

#ifndef UNIX_NETWORK_PROGRAMING_LIB_H
#define UNIX_NETWORK_PROGRAMING_LIB_H

#include "../libroute/unproute.h"
#include "../lib/unpifi.h"

char *net_rt_iflist_sysctl(int, int, size_t *);
//get_ifi_info函数(返回所有在工(即处于up状态)的接口)的sysctl版本
struct ifi_info *get_ifi_info_sysctl(int, int);

#endif //UNIX_NETWORK_PROGRAMING_LIB_H
