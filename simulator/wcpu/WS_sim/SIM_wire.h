#ifndef __SIM_WIRE_HEADER__
#define __SIM_WIRE_HEADER__

#include <stdint.h>

typedef struct SIM_wire
{
	uint64_t latency;
	uint64_t throughput;
	
}SIM_wire_t;

#endif