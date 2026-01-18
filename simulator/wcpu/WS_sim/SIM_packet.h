#ifndef __WS_SIM_PACKET_HEADER__
#define __WS_SIM_PACKET_HEADER__

#include "SIM_commons.h"

#include <stdint.h>
#include <stdalign.h>

typedef struct WS_SIM_packet
{
	_Alignas(8) uint8_t bytes[SIM_PACKET_SIZE];
}SIM_packet_t;

#endif