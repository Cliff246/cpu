#include "SIM_anchor.h"
#include "SIM_device.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

SIM_anchor_t *SIM_alloc_anchor(uint64_t count)
{
	assert(count > 0 && "cannot have anchor of 0");

	uint64_t size = sizeof(SIM_anchor_t) + (sizeof(SIM_device_t *) * count);

	SIM_anchor_t *anchor = calloc(1, size);
	assert(anchor);
	anchor->count = count;

	return anchor;

}

bool SIM_init_anchor(SIM_anchor_t *anchor)
{
	assert(0 && "TODO init anchor");
	return true;
}


void SIM_free_anchor(SIM_anchor_t *anchor)
{
	free(anchor);
}
