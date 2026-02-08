#include "SIM_packet.h"
#include "SIM_commons.h"



#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

SIM_packet_t SIM_packet_set(const uint8_t size)
{
	assert(size <= SIM_PACKET_MAX_SIZE && "set packet must be less than max packet size");
	SIM_packet_t packet =
	{
		.size = size,
		.bytes = {0},
		.used = 0
	};

	return packet;
}

bool SIM_packet_write(SIM_packet_t *packet, uint8_t used, uint8_t *bytes)
{
	assert(packet);
	assert(bytes);
	assert(used > SIM_PACKET_MAX_SIZE && "cannot exceed packet max size");
	assert(packet->bytes != bytes);
	if(packet->size < used || used == 0)
	{
		return false;
	}

	memcpy(packet->bytes, bytes, sizeof(uint8_t) * used);
	packet->used = used;
	return true;

}

//returns 0 to 1 and writes to bytes
uint8_t SIM_packet_read(SIM_packet_t *packet, uint8_t bytes[SIM_PACKET_MAX_SIZE])
{

	assert(packet);
	assert(bytes);
	assert(packet->bytes != bytes);

	assert(packet->size >= packet->used && packet->size <= SIM_PACKET_MAX_SIZE && "packet size cannot be invalid");


	memcpy(bytes, packet->bytes, packet->used);

	return packet->used;

}