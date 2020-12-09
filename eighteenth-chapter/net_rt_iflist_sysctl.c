//
// Created by 王勇椿 on 2020/12/8.
//
#include "lib.h"

char *
net_rt_iflist_sysctl(int family, int flags, size_t *lenp){

    int mib[6];
    char *buf;

    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = family; // only addresses of this family
    mib[4] = NET_RT_IFLIST;
    mib[5] = flags;

    //指定第三个参数为空指针的目的是:lenp参数是一个值-结果参数，函数返回时，lenp指向的变量就是存放所有接口信息所需缓冲区的大小
    if (Sysctl(mib, 6, NULL, lenp, NULL, 0) < 0){
        return NULL;
    }
    if ((buf = malloc(*lenp)) == NULL){
        return (NULL);
    }
    if (sysctl(mib, 6, buf, lenp, NULL, 0) < 0){
        free(buf);
        return (NULL);
    }

    return (buf);
}

