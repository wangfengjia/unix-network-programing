//
// Created by 王勇椿 on 2020/11/26.
//

#ifndef UNIX_NETWORK_PROGRAMING_LIB_H
#define UNIX_NETWORK_PROGRAMING_LIB_H

#include "../lib/unp.h"

//使用SIGALRM为connect函数设置超时
int connect_timeo_signal(int, const SA *, socklen_t, int);
//使用SIGALRM为recvfrom设置超时
void dg_cli_signal(FILE *, int, const SA *, socklen_t servlen);
void dg_cli_select(FILE *, int, const SA *, socklen_t);
//使用SO_RCVTIMEO套接字选项设置超时
void dg_cli_sopt(FILE *, int, const SA *, socklen_t);
//使用select为recvfrom设置超时
int readable_timeo_select(int fd, int sec);

#endif //UNIX_NETWORK_PROGRAMING_LIB_H
