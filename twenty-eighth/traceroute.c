//
// Created by 王勇椿 on 2020/12/25.
//
#include "traceroute.h"

struct proto proto_v4 = {icmpcode_v4, recv_v4, NULL, NULL, NULL, NULL, 0, IPPROTO_ICMP, IPPROTO_IP, IP_TTL};
#ifdef IPV6
struct proto proto_v6 = {icmpcode_v6, recv_v6, NULL, NULL, NULL, NULL, 0, IPPROTO_ICMPV6, IPPROTO_IPV6, IPV6_UNICAST_HOPS};
#endif

int datalen = sizeof(struct rec);
int max_ttl = 30;
int nprobes = 3;
ushort dport = 32768 + 666;

int
main(int argc, char *argv[]){

    int c;
    struct addrinfo *ai;
    char *h;

    opterr = 0; //don't want getopt() writing to stderr
    while ((c = getopt(argc, argv, "m:v")) != -1){
        switch (c) {
            case 'm':
                if ((max_ttl = atoi(optarg)) <= 1){
                    err_quit("invalid -m value");
                }
                break;
            case 'v':
                verbose++;
                break;
            case '?':
                err_quit("unrecognized option: %c", c);
        }
    }

    if (optind != argc - 1){
        err_quit("usage: traceroute [ -m <maxttl> -v ] hostname");
    }
    host = argv[optind];

    pid = getpid();
    Signal(SIGALRM, sig_alrm);

    ai = Host_serv(host, NULL, 0, 0);

}

