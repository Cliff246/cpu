#ifndef __WS_SIM_ROUTER_HEADER__
#define __WS_SIM_ROUTER_HEADER__







#include "SIM_channel.h"
#include <stdint.h>
#include <sys/types.h>

typedef struct WS_SIM_router
{
	uint64_t size;
	uint32_t start;
	uint32_t stop;
	SIM_channel_t **roundrobin;
}SIM_router_t;

void SIM_init_router(SIM_router_t *router, uint64_t size);

SIM_channel_t *SIM_router_dequeue_router(SIM_router_t *router);
bool SIM_router_enqueue_router(SIM_router_t *router, SIM_channel_t *channelptr);

#endif