#ifndef __WS_SIMULATOR_EVENT_HEADER__
#define __WS_SIMULATOR_EVENT_HEADER__

#include <stdint.h>
#include <stdbool.h>

#define SIM_MAX_PACKET_SIZE 16

typedef struct WS_SIM_packet
{
	uint8_t data[SIM_MAX_PACKET_SIZE];

	uint64_t wire;
	uint32_t time;
	uint16_t width;


}SIM_packet_t;




#endif
