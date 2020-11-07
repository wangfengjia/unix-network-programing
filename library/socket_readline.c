//
// Created by 王勇椿 on 2020/11/4.
//
#include "inet_lib.h"


static int read_cnt;
static char *read_ptr;
static char read_buf[MAXLINE];

static ssize_t my_read(int, char *);
ssize_t readlinebuf(void **vptrptr);

static ssize_t
my_read(int fd, char *ptr){

    if (read_cnt <= 0){
again:
        if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0){
            //系统调用被一个信号中断
            if (errno == EINTR){
                goto again;
            }
            return (-1);
        } else if (read_cnt == 0){
            return (0);
        }
        read_ptr = read_buf;
    }

    read_cnt--;
    *ptr = *read_ptr++;
    return (1);
}

ssize_t
readlinebuf(void **vptrptr){

    if (read_cnt){
        *vptrptr = read_ptr;
    }
    return read_cnt;
}


ssize_t
socket_readline(int fd, void *vptr, size_t maxlen){

    ssize_t n, rc;
    char c, *ptr;
    ptr = vptr;

    for (n = 1; n < maxlen; n++){
        if ((rc = my_read(fd, &c)) == 1){
            *ptr++ = c;
            if (c == '\n'){ //newline is stored, like fgets()
                break;
            }
        } else if (rc == 0){ //EOF, n - 1 bytes were read
            *ptr = 0;
            return (n - 1);
        } else{ // error, error set by read()
            return (-1);
        }
    }

    *ptr = 0; //null terminate like fgets()
    return (n);
}

