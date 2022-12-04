//
// Created by TempUser on 2022/12/4.
//

#include "errno.h"
#include "echo_skt.h"
#include "netinet/ip.h"
#include "netinet/ip_icmp.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

int open_echo_skt(struct echo_skt *skt, int mtu) {
    skt->buf = skt->data = NULL;

    /* open the icmp socket. */
    if ((skt->fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        fprintf(stderr, "unable to open icmp socket=>%s\n", strerror(errno));
        return -1;
    }

    /* calculate the buffer size required to encapsulate this payload. */
    skt->bufsize = mtu + sizeof(struct iphdr) + sizeof(struct icmphdr);

    /* allocate the buffer. */
    if ((skt->buf = malloc(skt->bufsize)) == NULL) {
        fprintf(stderr, "unable to allocate icmp tx/rx buffers=>%s\n", strerror(errno));
        return -1;
    }

    /* save a pointer to the icmp payload for convenience. */
    skt->data = skt->buf + sizeof(struct iphdr) + sizeof(struct icmphdr);

    return 0;
}
