//
// Created by TempUser on 2022/12/4.
//
#include "server.h"
#include "echo_skt.h"
#include "options.h"
#include "protocol.h"
#include "stdio.h"

int server(struct options *opts) {
    struct echo_skt skt;
    struct tun

    int bufsize = opts->mtu + sizeof(struct packet_header);

    if (open_echo_skt(&skt, bufsize) != 0) {
        return -1;
    }


}