#ifndef __WS_SIM_WIREROUTER_HEADER__
#define __WS_SIM_WIREROUTER_HEADER__







#include "SIM_channel.h"
#include <stdint.h>
#include <sys/types.h>

typedef struct WS_SIM_wirerouter
{
	uint64_t size;
	uint32_t start;
	uint32_t stop;
	SIM_channel_t **roundrobin;
}SIM_wirerouter_t;

void SIM_init_router(SIM_wirerouter_t *router, uint64_t size);

SIM_channel_t *SIM_dequeue_router(SIM_wirerouter_t *router);
bool SIM_enqueue_router(SIM_wirerouter_t *router, SIM_channel_t *channelptr);

#endif