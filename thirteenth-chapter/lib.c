//
// Created by 王勇椿 on 2020/11/24.
//
#include "../lib/unp.h"
#include <syslog.h>
#include "lib.h"

extern int daemon_proc; // defined in error.c
int
ldaemon_init(const char *pathname, int facility){

    int i;
    pid_t pid;

    if ((pid = Fork()) < 0){
        return (-1);
    } else if (pid){ //parent process,parent terminate
        _exit(0);
    }

    //child 1 continue
    //子进程创建一个新的会话，子进程成为新会话的会话首进程和新进程组的进程组首进程，从而不再有控制终端
    if (setsid() < 0){
        return (-1);
    }

    /**
     * 忽略SIGHUP的原因是:当会话首进程(第一次fork产生的子进程)终止时，其会话中的所有进程(即再次fork产生的子进程)都会收到SIGHUP信号
     */
    Signal(SIGHUP, SIG_IGN);

    /**
     * 再次fork的目的是确保本守护进程将来即使打开了一个终端设备，也不会自动获得控制终端。当没有控制终端的会话首进程打开一个终端设备，
     * 这个终端自动成为会话首进程的控制终端，然而再次调用fork之后，新的子进程不再是会话首进程，从而不能自动获得一个控制终端
     */
    if ((pid = Fork()) < 0){
        return (-1);
    } else if (pid){
        _exit(0); //child 1 terminate
    }

    //child 2 continue
    //将这个变量置为非0的目的是为了告知err_XXX()函数改为调用syslog,以取代fprint到标准错误输出
    daemon_proc = 1; //for err_XXX() functions
    //改变工作目录的原因:守护进程可能在某个任意的文件系统中启动，如果仍然在其中，那么这个文件系统就无法拆卸，除非使用潜在的破坏性的强制措施
    chdir("/"); //change working directory
    //close off file descriptors
    for (i = 0; i < MAXFD; i++){
        close(i);
    }

    //redirect stdin/stdout/stderr to /dev/null
    //打开这些描述符的理由:守护进程调用那些能从标准输入读或者标准输出或标准错误写的库函数将不会因为这些描述符未打开而失败
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    openlog(pathname, LOG_PID, facility);
    return (0); //success
}
