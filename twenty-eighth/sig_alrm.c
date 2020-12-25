//
// Created by 王勇椿 on 2020/12/24.
//

#include "ping.h"

void
sig_alrm(int signo){

    (*pr->sasend)();

    alarm(1);
    return;
}
