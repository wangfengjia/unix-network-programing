//
// Created by 王勇椿 on 2020/12/5.
//Our own header for the programs that need interface configuration info , include this file
//

#ifndef UNIX_NETWORK_PROGRAMING_LIB_UNPIFI_H
#define UNIX_NETWORK_PROGRAMING_LIB_UNPIFI_H

#include "../lib/unp.h"
#include <net/if.h>

#define LIB_IFI_NAME 16 //same as IFNAMSIZ in <net/if.h>
#define LIB_IFI_HADDR 8 //allow for 64-bit EUI-64 in future

struct lib_ifi_info{
    char ifi_name[LIB_IFI_NAME]; //interface name, null-terminated
    short ifi_index; //interface index
    short ifi_mtu; //interface MTU
    u_char ifi_haddr[LIB_IFI_HADDR]; //hardware address
    u_short ifi_hlen; // # bytes in hardware address: 0,6,8
    short ifi_flags; //IFF_XXX constants from <net/if.h>
    short ifi_myflags; // our own IFI_xxx flags
    struct sockaddr *ifi_addr; // primary address
    struct sockaddr *ifi_brdaddr; //broadcast address
    struct sockaddr *ifi_dstaddr; //destination address
    struct lib_ifi_info *ifi_next; // next of these structures
};

#define IFI_ALIAS 1 //ifi_addr is an alias

struct lib_ifi_info *lib_get_ifi_info(int, int);
struct lib_ifi_info *Lib_get_ifi_info(int, int);
void lib_free_ifi_info(struct lib_ifi_info *);

#endif //UNIX_NETWORK_PROGRAMING_LIB_UNPIFI_H
