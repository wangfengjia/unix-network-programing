//
// Created by 王勇椿 on 2020/12/3.
//
/**
 * strcli父子进程版本:
 *      1. 父进程负责读取来自标准输入，并将读取到的数据发送到服务器
 *      2. 子进程负责读取来自服务器的文本行，并把读取到文本行写到标准输出
 */

#include "lib.h"

void
strcli_fork(FILE *fp, int sockfd){

    pid_t pid;
    char sendline[MAXLINE], recvline[MAXLINE];

    if ((pid = fork()) == 0){ //child: server -> stdout
        while (Readline(sockfd, recvline, MAXLINE) > 0){
            Fputs(recvline, stdout);
        }
        kill(getppid(), SIGTERM); // in case parent still running
        exit(0);
    }

    // parent process: stdin -> server
    while (Fgets(sendline, MAXLINE, fp) != NULL){
        Writen(sockfd, sendline, strlen(sendline));
    }
    Shutdown(sockfd, SHUT_WR); // EOF on stdin, send FIN
    //父进程完成数据复制后调用pause让自己进入睡眠状态，直到捕获一个信号(子进程来的SIGTERM信号)，尽管它不主动捕获任何信号，SIGTERM信号的默认行为是终止进程
    pause();
    return;
}
