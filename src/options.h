//
// Created by TempUser on 2022/12/4.
//

#ifndef ICMPTUNNEL_OPTIONS_H
#define ICMPTUNNEL_OPTIONS_H

struct options {
    /* interval between keep-alive packets. */
    int keepalive;

    /* number of retries before time out. */
    int retries;

    /* tunnel mtu. */
    int mtu;

    /* enable windows ping emulation. */
    int emulation;

    /* run as a daemon. */
    int daemon;
};

#endif //ICMPTUNNEL_OPTIONS_H
