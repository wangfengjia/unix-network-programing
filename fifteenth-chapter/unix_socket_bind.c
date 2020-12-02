//
// Created by 王勇椿 on 2020/11/29.
//
//给一个unix套接字bind一个路径名

#include "../lib/unp.h"

int
main(int argc, char *argv[]){

    int sockfd;
    socklen_t len;
    struct sockaddr_un addr1, addr2;

    if (argc != 2){
        err_quit("usage: unixbind <pathname>");
    }

    sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);

    /**
     * 先调用unlink删除要给套接字绑定的路径名的目的是:以防它已经存在，如果文件系统中已经存在这个路径名，bind将会失败
     */
    unlink(argv[1]);

    /**
     * 将套接字地址结构初始化为0，并且从sun_path数组的大小中减1，可以肯定该路径名将以空字符结尾
     */
    bzero(&addr1, sizeof(addr1));
    addr1.sun_family = AF_LOCAL;
    //使用strncpy复制命令行参数，以免路径名过长导致其溢出结构
    strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path) - 1);
    Bind(sockfd, (SA *)&addr1, SUN_LEN(&addr1));

    len = sizeof(addr2);
    //获取刚刚绑定的路径名
    Getsockname(sockfd, (SA *)&addr2, &len);
    printf("bound name = %s, returned len = %d\n", addr2.sun_path, len);
    exit(0);
}
