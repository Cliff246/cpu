#ifndef __WS_SIM_WIRECONFIG_HEADER__
#define __WS_SIM_WIRECONFIG_HEADER__

#include "SIM_chnlcfg.h"
#include <stdint.h>

typedef struct WS_SIM_wirecfg
{
	SIM_wireid_t id;
	uint64_t latency;
	uint64_t throughput;
	uint64_t channels_alloc;
	uint64_t channels_count;
	SIM_chnlid_t *channels;
	struct
	{
		SIM_wire_t *transition;
		bool added_channels;
		bool finished;
	}done;
}SIM_wirecfg_t;


void SIM_init_wirecfg(SIM_wirecfg_t *ptr, SIM_wireid_t id, uint64_t latency, uint64_t throughput);

void SIM_print_wirecfg(SIM_wirecfg_t *wireconfig);
void SIM_append_wirecfg(SIM_wirecfg_t *wireconfig, SIM_chnlid_t cid);


void SIM_free_wirecfg(SIM_wirecfg_t *ptr);


#endif