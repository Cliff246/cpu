#include "SIM_stage.h"


#include "SIM_graph.h"
#include "OBJ_bundle.h"
#include "SIM_channel.h"
#include "SIM_chnlcfg.h"
#include "SIM_commons.h"
#include "SIM_context.h"
#include "SIM_device.h"
#include "SIM_mailbox.h"
#include "SIM_packet.h"
#include "SIM_port.h"
#include "SIM_transfer.h"
#include "SIM_wire.h"
#include "SIM_wirecfg.h"
#include "commons.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>


void SIM_init_stage_devices(SIM_stage_t *stage, SIM_context_t *context)
{

	for(int i = 0; i < stage->devices_count; ++i)
	{
		SIM_init_device(&stage->devices[i], &context->deviceconfigs->cfgs[i]);
	}
}


SIM_stage_t *SIM_init_stage(SIM_context_t *context)
{

	SIM_stage_t *stage = calloc(1, sizeof(SIM_stage_t));
	assert(stage);

	uint64_t devices_count = context->deviceconfigs->count;
	SIM_device_t *devices = calloc(devices_count, sizeof(SIM_device_t));
	assert(devices);
	stage->devices = devices;
	stage->devices_count =devices_count;

	SIM_init_stage_devices(stage, context);

	uint64_t wires_count = context->wireconfigs->count;
	SIM_wire_t *wires = calloc(wires_count, sizeof(SIM_wire_t));
	assert(wires);
	stage->wires = wires;
	stage->wires_count = wires_count;

	uint64_t channels_count = SIM_get_count_chnlcfg_buf(context->channelbuf);
	SIM_channel_t *channels = calloc(channels_count, sizeof(SIM_channel_t));
	assert(channels);

	stage->channels = channels;
	stage->channels_count = channels_count;
	SIM_free_context(context);
	return stage;
}