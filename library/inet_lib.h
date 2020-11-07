//
// Created by 王勇椿 on 2020/11/4.
//

#ifndef UNIX_NETWORK_PROGRAMING_INET_LIB_H
#define UNIX_NETWORK_PROGRAMING_INET_LIB_H
#include <net/if_dl.h>
#include "../lib/unp.h"

char *socket_ntop(const struct sockaddr *, socklen_t);

//从tcp套接字读取指定字节数的数据，只有当读取到了指定字节的数据后函数才返回
ssize_t socket_readn(int, void *, size_t);
//写指定字节数的数据到tcp套接字，只有指定数据都写成功后函数才返回
ssize_t socket_writen(int, const void *, size_t);

/**
 * 基于文本行的协议很多，比如SMTP/HTTP/FTP的控制协议，所以就出现了针对文本行进行操作的需求,然后还是依照缓冲区而不是文本行的要求来编程
 * 编写从缓冲区中读取数据的代码，当期待一个文本行时，就查看缓冲区中是否含有这一行，socket_readline函数就是实现这个目的
 */
ssize_t socket_readline(int, void *, size_t);
#endif //UNIX_NETWORK_PROGRAMING_INET_LIB_H
