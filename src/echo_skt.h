//
// Created by TempUser on 2022/12/4.
//

#ifndef ICMPTUNNEL_ECHO_SKT_H
#define ICMPTUNNEL_ECHO_SKT_H

#include "stdint.h"

struct echo_skt {
    int fd;
    int bufsize;
    char *buf, *data;
};

struct echo {
    int size;
    int reply;
    uint16_t id;
    uint16_t seq;
};

/* open an icmp echo socket. */
int open_echo_skt(struct echo_skt *skt, int mtu);

/* send an echo packet. */
int send_echo(struct echo_skt *skt, uint32_t destip, struct echo *echo);

/* receive an echo packet. */
int receive_echo(struct echo_skt *skt, uint32_t *sourceip, struct echo *echo);

/* close the socket. */
void close_echo_skt(struct echo_skt *skt);

#endif //ICMPTUNNEL_ECHO_SKT_H
