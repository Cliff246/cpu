#ifndef __WS_SIM_STAGE_HEADER__
#define __WS_SIM_STAGE_HEADER__


#include "SIM_channel.h"
#include "SIM_device.h"
#include "SIM_wire.h"
#include "SIM_context.h"
#include "SIM_vistable.h"
#include <stdint.h>


typedef struct WS_SIM_stage
{
	uint32_t channels_count;
	uint32_t devices_count;
	uint32_t wires_count;

	SIM_channel_t *channels;

	SIM_device_t *devices;

	SIM_wire_t *wires;

}SIM_stage_t;

SIM_stage_t *SIM_init_stage(SIM_context_t *context);


#endif