#ifndef __WS_SIM_WIRECONFIG_HEADER__
#define __WS_SIM_WIRECONFIG_HEADER__

#include "SIM_channel.h"
#include <stdint.h>

typedef struct WS_SIM_wirecfg
{
	uint64_t id;
	uint64_t latency;
	uint64_t throughput;
	uint64_t channels;
	struct
	{
		SIM_wire_t *transition;
		bool finished;
	}done;
}SIM_wirecfg_t;


void SIM_init_wirecfg(SIM_wirecfg_t *ptr, uint64_t id, uint64_t latency, uint64_t throughput);

void SIM_wireconfig_print(SIM_wirecfg_t *wireconfig);
void SIM_free_wirecfg(SIM_wirecfg_t *ptr);


#endif