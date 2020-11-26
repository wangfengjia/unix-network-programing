//
// Created by 王勇椿 on 2020/11/24.
//

#ifndef UNIX_NETWORK_PROGRAMING_LIB_H
#define UNIX_NETWORK_PROGRAMING_LIB_H

#define MAXFD 64
//把一个普通进程转变为守护进程
int ldaemon_init(const char *, int);

#endif //UNIX_NETWORK_PROGRAMING_LIB_H
