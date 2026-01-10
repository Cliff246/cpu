#ifndef __WS_OBJ_PACKET_HEADER__
#define __WS_OBJ_PACKET_HEADER__

#include <stdlib.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct WS_OBJ_packet_block
{
	uint64_t block;
	_Alignas(8) uint8_t bytes[];
}OBJ_pktblk_t;

typedef struct WS_OBJ_packet
{
	size_t split;
	size_t size;
	_Alignas(8) OBJ_pktblk_t blocks[];
}OBJ_pkt_t;

#endif