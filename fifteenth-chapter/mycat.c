//
// Created by 王勇椿 on 2020/12/1.
//
//将一个文件复制到标准输出

#include "../lib/unp.h"
#include <stddef.h>

//打开一个文件并返回其描述符
int my_open(const char *, int);
ssize_t lread_fd(int, void *, size_t, int *);
ssize_t lwrite_fd(int, void *, size_t, int);

int
main(int argc, char *argv[]){


}

int
my_open(const char *pathname, int mode){

    int fd, sockfd[2], status;
    pid_t childpid;
    char c, argsockfd[10], argmode[10];

    Socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);
    if ((childpid = fork()) == 0){ //child process
        Close(sockfd[0]);
        snprintf(argsockfd, sizeof(argsockfd), "%d", sockfd[1]);
        snprintf(argmode, sizeof(argmode), "%d", mode);
        execl("./openfile", "openfile", argsockfd, pathname, argmode, (char *)NULL);
        err_sys("execl error");
    }

    //parent process - wait for the child to terminate
    Close(sockfd[1]); //close the end we don't use
    Waitpid(childpid, &status, 0);
    if (WIFEXITED(status) == 0){
        err_quit("child did not terminate");
    }
    if ((status = WEXITSTATUS(status)) == 0){
        Read_fd(sockfd[0], &c, 1, &fd);
    } else{
        //set errno value from child's status
        errno = status;
        fd = -1;
    }

    Close(sockfd[0]);
    exit(0);
}

ssize_t
lread_fd(int fd, void *ptr, size_t nbytes, int *recvfd){

    struct msghdr msg;
    struct iovec iov[1];
    ssize_t n;

#ifdef HAVE_MSGHDR_MSG_CONTROL
    union {
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof(int))];
    } control_un;
    struct cmsghdr *cmptr;

    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);
#else
    int newfd;
    msg.msg_accrights = (caddr_t) &newfd;
    msg.msg_accrightslen = sizeof(int);
#endif

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    if ((n = recvmsg(fd, &msg, 0)) <= 0){
        return (n);
    }

#ifdef HAVE_MSGHDR_MSG_CONTROL
    if ((cmptr = CMSG_FIRSTHDR(&msg)) != NULL && cmptr->cmsg_len == CMSG_LEN(sizeof(int))){
        if (cmptr->cmsg_level != SOL_SOCKET){
            err_quit("control level != SOL_SOCKET");
        }
        if (cmptr->cmsg_type != SCM_RIGHTS){
            err_quit("control type != SCM_RIGHTS");
        }
        *recvfd = *((int *) CMSG_DATA(cmptr));
    } else{
        *recvfd = -1; // descriptor was not passed
    }
#else
    if (msg.msg_accrightslen == sizeof(int)){
        *recvfd = newfd;
    }else{
        *recvfd = -1;
    }
#endif

    return (n);
}

ssize_t
lwrite_fd(int fd, void *ptr, size_t nbytes, int sendfd){

    struct msghdr msg;
    struct iovec iov[1];

#ifdef HAVE_MSGHDR_MSG_CONTROL
    union {
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof(int))];
    } control_un;

    struct cmsghdr *cmptr;
    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);
    cmptr = CMSG_FIRSTHDR(&msg);
    cmptr->cmsg_len = CMSG_LEN(sizeof(int));
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;
    *((int *) CMSG_DATA(cmptr)) = sendfd;
#else
    msg.msg_accrights = (caddr_t) &sendfd;
    msg.msg_accrightslen = sizeof(int);
#endif
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;


    return (sendmsg(fd, &msg, 0));
}
