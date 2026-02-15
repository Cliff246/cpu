#include "SIM_stage.h"


#include "CFG_map.h"
#include "OBJ_bundle.h"
#include "CFG_link.h"
#include "RUN_graph.h"
#include "SIM_commons.h"
#include "CFG_context.h"
#include "SIM_device.h"
#include "CFG_edge.h"
#include "commons.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>


static void SIM_init_stage_devices(SIM_stage_t *stage, CFG_context_t *context)
{
	uint64_t counter = 0;
	for(int i = 0; i < stage->devices_count; ++i)
	{
		SIM_device_t *dev = &stage->devices[i];
		dev->id = counter++;
		SIM_init_device(dev, &context->deviceconfigs->cfgs[i]);
	}
}


SIM_stage_t *SIM_init_stage(CFG_context_t *context)
{

	SIM_stage_t *stage = calloc(1, sizeof(SIM_stage_t));
	assert(stage);





	uint64_t devices_count = context->deviceconfigs->count;
	SIM_device_t *devices = calloc(devices_count, sizeof(SIM_device_t));
	assert(devices);
	stage->devices = devices;
	stage->devices_count =devices_count;

	SIM_init_stage_devices(stage, context);

	return stage;
}