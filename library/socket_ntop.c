//
// Created by 王勇椿 on 2020/11/4.
//
#include "inet_lib.h"
#include <stddef.h>

char
*socket_ntop(const struct sockaddr *addr, socklen_t salen){

    char portstr[8];
    static char str[128];

    switch (addr->sa_family) {
        case AF_INET:{
            struct sockaddr_in *sin = (struct sockaddr_in *)addr;
            if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL){
                return NULL;
            }
            if (ntohs(sin->sin_port) != 0){
                snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
                strcat(str, portstr);
            }

            return str;
        }
    }
}
