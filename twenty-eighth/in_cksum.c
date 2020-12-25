//
// Created by 王勇椿 on 2020/12/24.
//

#include "ping.h"

uint16_t
in_cksum(uint16_t *addr, int len){

    int nleft = len;
    uint32_t sum = 0;
    uint16_t *w = addr;
    uint16_t answer = 0;

    /**
     * algorithm: using a 32 bit accumulator(sum), we add sequential 16 bit words to it. and at the end, fold back all
     * the carry bits from top 16 bits into the lower of 16 bits
     */
    while (nleft > 1){
        sum += *w++;
        nleft -= 2;
    }

    // mop up an odd byte, if necessary
    if (nleft == 1){
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }

    // add back carry outs from top 16 bits to low bits
    sum = (sum >> 16) + (sum & 0xffff); // add hi 16 to low 16
    sum += (sum >> 16); //add carry
    answer -= sum; // truncate to 16 bits
    return answer;
}

