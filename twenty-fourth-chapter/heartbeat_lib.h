//
// Created by 王勇椿 on 2020/12/20.
//

#ifndef UNIX_NETWORK_PROGRAMING_HEARTBEAT_LIB_H
#define UNIX_NETWORK_PROGRAMING_HEARTBEAT_LIB_H

/**
 * 客户/服务器的心跳函数
 * 心跳函数实现:客户每隔一秒向服务器发送一个带外字节，服务器收到这个带外字节后向客户发送回一个带外字节。客户和服务器每隔一秒钟递增它们的cnt变量一次，
 * 每收到一个带外字节又把这个变量置为0，如果这个计数器达到5(也就说本进程已有5秒钟没有收到来自对端的带外字节)，就认定连接失效。当有带外字节到达时，
 * 客户和服务器都使用SIGURG信号得以通知。正常数据和带外字节都通过单个TCP连接交换
 */

#include "../lib/unp.h"

void myheartbeat_cli(int, int, int);
void myheartbeat_serv(int, int, int);
#endif //UNIX_NETWORK_PROGRAMING_HEARTBEAT_LIB_H
