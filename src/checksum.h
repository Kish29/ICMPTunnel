//
// Created by TempUser on 2022/12/5.
//

#ifndef ICMPTUNNEL_CHECKSUM_H
#define ICMPTUNNEL_CHECKSUM_H

#include "stdint.h"

/* calculate an icmp checksum. */
uint16_t checksum(const char *buf, int size);

#endif //ICMPTUNNEL_CHECKSUM_H
