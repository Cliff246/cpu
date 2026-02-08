#ifndef __WS_SIM_PACKET_HEADER__
#define __WS_SIM_PACKET_HEADER__

#include "OBJ_message.h"
#include "SIM_commons.h"

#include <stdint.h>
#include <stdalign.h>
#include <stdbool.h>


typedef struct WS_SIM_packet
{
	_Alignas(64) uint8_t bytes[SIM_PACKET_MAX_SIZE];
	uint8_t used;
	uint8_t size;


}SIM_packet_t;


SIM_packet_t SIM_packet_set(const uint8_t size);
bool SIM_packet_write(SIM_packet_t *packet, uint8_t used, uint8_t *bytes);

uint8_t SIM_packet_read(SIM_packet_t *packet, uint8_t bytes[SIM_PACKET_MAX_SIZE]);



#endif