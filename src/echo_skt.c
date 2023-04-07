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
#include "checksum.h"
#include "unistd.h"

int open_echo_skt(struct echo_skt *skt, int mtu) {
    skt->buf = skt->data = NULL;

    /* open the icmp socket. */
    if ((skt->fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        fprintf(stderr, "unable to open icmp socket=>%s\n", strerror(errno));
        return -1;
    }

    /* calculate the buffer size required to encapsulate this payload. */
    /* data size(mtu) + ip header size(iphdr) + icmp header size(icmphdr). */
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

/* send an echo packet. */
int send_echo(struct echo_skt *skt, uint32_t destip, struct echo *echo) {
    ssize_t xfer;

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = htonl(destip);
    dest.sin_port = 0;  /* valgrind check. :) */

    /* write icmp header */
    struct icmphdr *header = (struct icmphdr *) (skt->buf + sizeof(struct icmphdr));
    /* https://klose911.github.io/html/tii/icmp.html */
    header->type = echo->reply ? ICMP_ECHOREPLY : ICMP_ECHO;
    header->code = 0;
    header->un.echo.id = echo->id;
    header->un.echo.sequence = echo->seq;
    header->checksum = checksum(skt->buf + sizeof(struct iphdr), sizeof(struct icmphdr) + echo->size);

    /* send echo packet. */
    xfer = sendto(skt->fd,
                  skt->buf + sizeof(struct iphdr),
                  sizeof(struct icmphdr) + echo->size,
                  0, (struct sockaddr *) &dest,
                  sizeof(struct sockaddr_in));

    if (xfer < 0) {
        fprintf(stderr, "unable to send icmp packet, err=>%s", strerror(errno));
        return -1;
    }
    return 0;
}

/* receive an echo packet. */
int receive_echo(struct echo_skt *skt, uint32_t *sourceip, struct echo *echo) {
    ssize_t xfer;
    struct sockaddr_in source;
    socklen_t source_size = sizeof(struct sockaddr_in);

    xfer = recvfrom(skt->fd, skt->buf, skt->bufsize, 0, (struct sockaddr *) &source, &source_size);

    if (xfer < 0) {
        fprintf(stderr, "unable to receive icmp packet, err=>%s", strerror(errno));
        return -1;
    }

    struct icmphdr *header = (struct icmphdr *) (skt->buf + sizeof(struct iphdr));
    ssize_t ipicmp_hdrsize = sizeof(struct iphdr) + sizeof(struct icmphdr);
    if (xfer < ipicmp_hdrsize) {
        fprintf(stderr, "receive icmp packet size illegal, size=>%ld", ipicmp_hdrsize);
        return -1;
    }

    if ((header->type != ICMP_ECHOREPLY && header->type != ICMP_ECHO) || header->code != 0) {
        fprintf(stderr, "not supported type and code combination, type=>%d, code=>%d", header->type, header->code);
        return -1;
    }

    *sourceip = ntohl(source.sin_addr.s_addr);
    echo->size = xfer - ipicmp_hdrsize;
    echo->reply = header->type == ICMP_ECHOREPLY;
    echo->id = ntohs(header->un.echo.id);
    echo->seq = ntohs(header->un.echo.sequence);

    return 0;
}

/* close the socket. */
void close_echo_skt(struct echo_skt *skt) {
    /* dispose of the buffer. */
    if (skt->buf) {
        free(skt->buf);
    }
    /* close the icmp socket*/
    if (skt->fd >= 0) {
        close(skt->fd);
    }
}
