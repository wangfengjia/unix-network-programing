//
// Created by 王勇椿 on 2020/12/22.
//
//套接字是阻塞模式，为每个连接创建一个线程
#include "lib.h"
#include <stdbool.h>

#define MAXFILES 20
#define SERV "80"  //port number or service name

struct file{
    char *f_name; // filename
    char *f_host; // hostname or IP address
    int f_fd; // descriptor
    int f_flags; // F_XX belows
    pthread_t f_tid;
} files[MAXFILES];
#define F_CONNECTING 1 //connect() in the process
#define F_READING 2 //connect() complete; now reading
#define F_DONE 4 //all done

#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"

int nconn, nfiles, nlefttoconn, nlefttoread;

//建立TCP连接，给服务器发送一个HTTP GET命令，并读入来自服务器的应答
void *do_get_read(void *);
void home_page(const char *, const char *);
void write_get_cmd(struct file *);

int
main(int argc, char *argv[]){

    int i, n, maxnconn;
    pthread_t tid;
    struct file *fptr;

    if (argc < 5){
        err_quit("usage: web <#conns> <IPaddr> <homepage> file1 ...");
    }
    maxnconn = atoi(argv[1]);

    nfiles = min(argc - 4, MAXFILES);
    for (i = 0; i < nfiles; i++){
        files[i].f_name = argv[i + 4];
        files[i].f_host = argv[2];
        files[i].f_flags = 0;
    }

    printf("nfiles = %d\n", nfiles);
    home_page(argv[2], argv[3]);

    nlefttoread = nlefttoconn = nfiles;
    nconn = 0;

    while (nlefttoread > 0){
        while (nconn < maxnconn && nlefttoconn > 0){
            // find a file to read
            for (i = 0; i < nfiles; i++){
                if (files[i].f_flags == 0){
                    break;
                }
            }
            if (i == nfiles){
                err_quit("nlefttoconn = %d but nothing found", nlefttoconn);
            }
            files[i].f_flags = F_CONNECTING;
            Pthread_create(&tid, NULL, &do_get_read, &files[i]);
            files[i].f_tid = tid;
            nconn++;
            nlefttoconn--;
        }

        //todo
//        if ((n = pthread_join()))
        nconn--;
        nlefttoread--;
        printf("thread id %d for %s done\n", tid, fptr->f_name);
    }

    exit(0);
}

void *
do_get_read(void *vptr){

    int fd, n;
    char line[MAXLINE];
    struct file *fptr;

    fptr = (struct file *)vptr;

    fd = Tcp_connect(fptr->f_host, SERV);
    fptr->f_fd = fd;
    printf("do_get_read for %s, fd %d, thread %d\n", fptr->f_name, fptr->f_tid);

    //给服务器发送GET命令
    while (true){

        if ((n = Read(fd, line, MAXLINE)) == 0){
            break; // server closed connection
        }
        printf("read %d bytes from %s\n", n, fptr->f_name);
    }
    printf("end-of-file on %s\n", fptr->f_name);
    Close(fd);
    fptr->f_flags = F_DONE; // clears F_READING
    return fptr;
}

void
home_page(const char *host, const char *fname){

    int fd, n;
    char line[MAXLINE];

    fd = Tcp_connect(host, SERV);
    n = snprintf(line, sizeof(line), GET_CMD, fname);
    Writen(fd, line, n);

    while (true){
        if ((n = Read(fd, line, MAXLINE)) == 0){
            break; // server closed connection
        }
        printf("read %d bytes of home page\n", n);
    }

    printf("end-of-file on home page\n");
    Close(fd);
}



