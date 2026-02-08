#ifndef __WS_SIM_PACKETBUFFER_HEADER__
#define __WS_SIM_PACKETBUFFER_HEADER__

#include "SIM_packet.h"
#include <stdint.h>
#include <stdalign.h>

typedef struct WS_SIM_packetbuffer
{
	SIM_packet_t *packets;

	uint64_t size;
	uint64_t count;
}SIM_packetbuffer_t;


SIM_packetbuffer_t *SIM_init_packetbuffer(uint64_t count, uint64_t size);


#endif