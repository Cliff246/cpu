#ifndef __WS_SIMULATOR_EVENT_HEADER__
#define __WS_SIMULATOR_EVENT_HEADER__

#include <stdint.h>
#include <stdbool.h>

#define SIM_MAX_PACKET_SIZE 16

typedef struct WS_SIM_packet
{
	int width;
	char data[SIM_MAX_PACKET_SIZE];
}SIM_packet_t;




#endif