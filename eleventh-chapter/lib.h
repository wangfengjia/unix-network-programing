//
// Created by 王勇椿 on 2020/11/21.
// 主机名字和IP地址转换章节的类库
//

#ifndef UNIX_NETWORK_PROGRAMING_OB_H
#define UNIX_NETWORK_PROGRAMING_OB_H

#include "../lib/unp.h"

//使用getaddrinfo处理TCP客户端和服务器大多数情形的两个函数
int ltcp_connect(const char *host, const char *serv);
int ltcp_listen(const char *, const char *, socklen_t *);
//用于创建未连接的UDP套接字
int ludp_client(const char *, const char *, SA **, socklen_t *);
//用于创建已连接UDP套接字,有了已连接UDP套接字后，udp_client必需的结尾两个参数就不再需要了，调用者可改用write代替sendto,因此也不必返回一个套接字地址结构及其长度
int ludp_connect(const char *, const char *);

int ludp_server(const char *, const char *, socklen_t *);

#endif //UNIX_NETWORK_PROGRAMING_OB_H
