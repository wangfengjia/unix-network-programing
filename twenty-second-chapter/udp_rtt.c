//
// Created by 王勇椿 on 2020/12/14.
//
#include "udp_reliability.h"

int rtt_debug_flag = 0; //debug flag; can be set by caller
//calculate the RTO value based on current estimators:smoothed RTT plus four times the deviation
#define RTT_RTOCALC(ptr) ((ptr)->rtt_srtt + (4.0 * (ptr)->rtt_rttvar))
static float udp_rtt_minmax(float);

static float
udp_rtt_minmax(float rto){

    if (rto < RTT_RXTMIN){
        rto = RTT_RXTMIN;
    } else if (rto > RTT_RXTMAX){
        rto = RTT_RXTMAX;
    }

    return rto;
}

void
udp_rtt_init(struct rtt_info *ptr){

    struct timeval tv;
    Gettimeofday(&tv, NULL);
    ptr->rtt_base = tv.tv_sec;
    ptr->rtt_rtt = 0;
    ptr->rtt_srtt = 0;
    ptr->rtt_rto = udp_rtt_minmax(RTT_RTOCALC(ptr));
}

uint32_t
udp_rtt_ts(struct rtt_info *ptr){

    uint32_t ts;
    struct timeval tv;
    Gettimeofday(&tv, NULL);

    ts = ((tv.tv_sec - ptr->rtt_base) * 1000) + (tv.tv_usec / 1000);
    return (ts);
}

void
udp_rtt_newpack(struct rtt_info *ptr){
    ptr->rtt_nrexmt = 0;
}

int
udp_rtt_start(struct rtt_info *ptr){

    return ((int) (ptr->rtt_rto + 0.5));
}

void
udp_rtt_stop(struct rtt_info *ptr, uint32_t ms){

    double delta;

    ptr->rtt_rtt = ms / 1000.0;
    //Update our estimators of RTT and mean deviation of RTT
    delta = ptr->rtt_rtt - ptr->rtt_srtt;
    ptr->rtt_srtt += delta / 8; //g = 1/8
    if (delta < 0){
        delta = -delta;
    }
    ptr->rtt_rttvar += (delta - ptr->rtt_rttvar) / 4; //h = 1/4
    ptr->rtt_rto = udp_rtt_minmax(RTT_RTOCALC(ptr));
}

int
udp_rtt_timeout(struct rtt_info *ptr){

    //next RTO
    ptr->rtt_rto *= 2;
    if (++ptr->rtt_nrexmt > RTT_MAXNREXMT){
        return (-1); //time to give up for this packet
    }
    return (0);
}
