//
// Created by 王勇椿 on 2020/12/26.
//
//显示总CPU时间

#include "lib.h"
#include <sys/resource.h>

#ifndef HAVE_GETRUSAGE_PROTO
int getruseage(int, struct rusage *);
#endif

#define USEC_TO_SEC 1000000.0

void
pr_cpu_time(void){

    double user, sys;
    struct rusage myusage, childusage;

    if (getrusage(RUSAGE_SELF, &myusage) < 0){
        err_sys("getrusage error");
    }
    if (getrusage(RUSAGE_CHILDREN, &childusage) < 0){
        err_sys("getrusage error");
    }

    user = (double) myusage.ru_utime.tv_sec + myusage.ru_utime.tv_usec / USEC_TO_SEC;
    user += (double) childusage.ru_utime.tv_sec + childusage.ru_utime.tv_usec / USEC_TO_SEC;
    sys = (double) myusage.ru_stime.tv_sec + myusage.ru_stime.tv_usec / USEC_TO_SEC;
    sys += (double) childusage.ru_stime.tv_sec + childusage.ru_stime.tv_usec / USEC_TO_SEC;

    printf("\nuser time = %g, sys time = %g\n", user, sys);
}
