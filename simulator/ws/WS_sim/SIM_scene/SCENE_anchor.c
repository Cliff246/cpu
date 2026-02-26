#include "SCENE_anchor.h"
#include "SCENE_device.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

SCENE_anchor_t *SCENE_alloc_anchor(uint64_t count)
{
	assert(count > 0 && "cannot have anchor of 0");

	uint64_t size = sizeof(SCENE_anchor_t) + (sizeof(SCENE_device_t *) * count);

	SCENE_anchor_t *anchor = calloc(1, size);
	assert(anchor);
	anchor->count = count;

	return anchor;

}

bool SCENE_init_anchor(SCENE_anchor_t *anchor)
{
	assert(0 && "TODO init anchor");
	return true;
}


void SCENE_free_anchor(SCENE_anchor_t *anchor)
{
	free(anchor);
}
