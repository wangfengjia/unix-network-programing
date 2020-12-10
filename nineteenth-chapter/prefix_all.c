//
// Created by 王勇椿 on 2020/12/10.
//
#include "lib.h"

int
prefix_all(struct sockaddr *sa){

    switch (sa->sa_family) {
        case AF_INET:
            return 32;
#ifdef IPV6
        case AF_INET6:
            return 128;
#endif
        default:
            return 0;
    }
}

