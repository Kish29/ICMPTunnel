//
// Created by TempUser on 2022/12/5.
//

#include "checksum.h"

uint16_t checksum(const char *buf, int size) {
    uint16_t *p = (uint16_t *) buf;
    uint32_t sum = 0;

    for (sum = 0; size > 1; size -= 2) {
        sum += *p++;
    }
    if (size == 1) {
        sum += *(unsigned char *) p;
    }

    /* high 16bits + low 16bits. */
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return ~sum;
}