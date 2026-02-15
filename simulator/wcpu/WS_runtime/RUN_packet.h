#ifndef __RUN_PACKET_HEADER__
#define __RUN_PACKET_HEADER__

#include <stdint.h>

#define RUN_PACKET_SIZE 8

typedef struct RUN_packet
{
	_Alignas(RUN_PACKET_SIZE) uint8_t bytes[RUN_PACKET_SIZE];
}RUN_pkt_t;

#endif