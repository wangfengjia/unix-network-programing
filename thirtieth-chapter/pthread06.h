//
// Created by 王勇椿 on 2020/12/27.
//

#ifndef UNIX_NETWORK_PROGRAMING_PTHREAD06_H
#define UNIX_NETWORK_PROGRAMING_PTHREAD06_H

#include "lib.h"

#define MAXNCLI 32
int clifd[MAXNCLI], iget, iput;
pthread_mutex_t clifd_mutex;
pthread_cond_t clifd_cond;


#endif //UNIX_NETWORK_PROGRAMING_PTHREAD06_H
