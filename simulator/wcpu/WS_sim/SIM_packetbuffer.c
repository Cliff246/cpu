#include "SIM_packetbuffer.h"

#include "SIM_commons.h"
#include "SIM_packet.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

SIM_packetbuffer_t *SIM_init_packetbuffer(uint64_t count, uint64_t size)
{
	SIM_packetbuffer_t *pktbuf = calloc(1, sizeof(SIM_packetbuffer_t));
	assert(pktbuf);
	SIM_packet_t *packets = calloc(count, sizeof(SIM_packet_t));
	assert(packets);

	assert(size <= SIM_PACKET_MAX_SIZE);

	for(uint64_t i = 0; i < count; ++i)
	{
		packets[i] = SIM_packet_set(size);
	}

	pktbuf->packets = packets;
	pktbuf->count = count;
	pktbuf->size = size;

	return pktbuf;
}