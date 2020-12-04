//
// Created by 王勇椿 on 2020/12/2.
//

#ifndef UNIX_NETWORK_PROGRAMING_LIB_H
#define UNIX_NETWORK_PROGRAMING_LIB_H

#include "../lib/unp.h"

//str_cli函数的非阻塞版本
void strcli_nonblock(FILE *, int);
void strcli_fork(FILE *, int sockfd);
int connect_nonblock(int, const SA *, socklen_t, int);
#endif //UNIX_NETWORK_PROGRAMING_LIB_H
