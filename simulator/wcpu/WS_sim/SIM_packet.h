#ifndef __WS_SIMULATOR_EVENT_HEADER__
#define __WS_SIMULATOR_EVENT_HEADER__

#include <stdint.h>
#include <stdbool.h>

typedef struct WS_SIM_packet
{
	int64_t control;
	int64_t data;
	int64_t address;
}SIM_packet_t;


#endif