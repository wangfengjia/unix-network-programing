//
// Created by 王勇椿 on 2020/12/26.
//

#ifndef UNIX_NETWORK_PROGRAMING_LIB_H
#define UNIX_NETWORK_PROGRAMING_LIB_H

#include "../lib/unp.h"
#include <stdbool.h>
//父进程管理子进程的信息结构

typedef struct {
    pid_t child_pid; // process ID
    int child_pipefd; //parent's stream pipe to/from child
    int child_status; // 0 = ready
    long child_count; // connection handled
} Child;

Child *cptr; // array of Child structures; calloc'ed
#define MAXN 16384 // max bytes to request from server

//显示总cpu时间
void pr_cpu_time(void);
//处理每一个客户请求
void web_child(int);
//派生各个子进程
pid_t child_make(int, int, int);
//派生各个子进程的第二个版本，父进程创建一对unix域套接字，父子进程通过unix域套接字通信
pid_t child_make_v2(int, int, int);

#endif //UNIX_NETWORK_PROGRAMING_LIB_H
