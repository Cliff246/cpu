#include "SIM_router.h"
#include "SIM_channel.h"
#include <assert.h>
#include <stdlib.h>

void SIM_init_router(SIM_router_t *router, uint64_t size)
{
	SIM_channel_t **roundrobin =  calloc(size, sizeof(SIM_channel_t *));
	assert(roundrobin);

	router->roundrobin = roundrobin;
	router->size = size;

}