#ifndef __WS_OBJ_PACKET_HEADER__
#define __WS_OBJ_PACKET_HEADER__

#include <stdlib.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdbool.h>

#define OBJ_PACKET_BLOCK_SIZE 64
#define OBJ_PACKET_BLOCK_COUNT 8
typedef struct WS_OBJ_packet_block
{
	int id;
	_Alignas(8) uint8_t block[OBJ_PACKET_BLOCK_SIZE];
}OBJ_pktblk_t;



typedef struct WS_OBJ_packet
{
	size_t split;
	size_t size;
	_Alignas(8) OBJ_pktblk_t blocks[OBJ_PACKET_BLOCK_COUNT];
}OBJ_pkt_t;

#endif
