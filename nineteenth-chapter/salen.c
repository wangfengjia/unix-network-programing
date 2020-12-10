//
// Created by 王勇椿 on 2020/12/10.
//
#include "lib.h"

int
salen(struct sockaddr *sa){

#ifdef HAVE_SOCKADDR_SA_LEN
    return sa->sa_len;
#else
    switch (sa->sa_family) {
        case AF_INET:
            return sizeof(struct sockaddr_in);
#ifdef IPV6
        case AF_INET6:
            return sizeof(struct sockaddr_in6);
#endif
        default:
            return 0;
    }

#endif
}

