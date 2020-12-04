//
// Created by 王勇椿 on 2020/12/3.
//

#ifndef UNIX_NETWORK_PROGRAMING_WEB_H
#define UNIX_NETWORK_PROGRAMING_WEB_H

#include "../lib/unp.h"

#define MAXFILES 20
#define SERV "80" //port number or service number

struct file{
    char *f_name; //filename
    char *f_host; //hostname or IPV4/IPv6 address
    int f_fd; //descriptor
    int f_flags; //F_xxx below
} file[MAXFILES];

#define F_CONNECTING 1 //connect() in progress
#define F_READING 2 //connect() complete;now reading
#define F_DONE 4 //all done

#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"

//globals
int nconn, nfiles, nlefttoconn, nlefttoread, maxfd;
fd_set rset, wset;

void home_page(const char *, const char *);
void start_connect(struct file *);
void write_get_cmd(struct file *);

#endif //UNIX_NETWORK_PROGRAMING_WEB_H
