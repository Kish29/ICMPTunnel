//
// Created by TempUser on 2022/12/5.
//

#ifndef ICMPTUNNEL_PROTOCOL_H
#define ICMPTUNNEL_PROTOCOL_H

#include "stdint.h"
/* conventions of icmp tunnel packet data format. */

/* magic value used to mark icmp tunnel packets. */
#define PACKET_MAGIC "TUNL"

enum PACKET_TYPE {
    PACKET_CONNECTION_REQUEST,
    PACKET_CONNECTION_ACCEPT,
    PACKET_SERVER_FULL,
    PACKET_DATA,
    PACKET_PUNCHTHRU,
    PACKET_KEEPALIVE
};

struct packet_header {
    uint8_t magic[4];
    uint8_t type;
};

#endif //ICMPTUNNEL_PROTOCOL_H
