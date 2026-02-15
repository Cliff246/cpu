#ifndef __WS_SIM_STAGE_HEADER__
#define __WS_SIM_STAGE_HEADER__


#include "CFG_map.h"
#include "CFG_context.h"
#include "SIM_wire.h"
#include "SIM_device.h"
#include <stdint.h>


typedef struct WS_SIM_stage
{
	uint32_t devices_count;
	uint32_t wires_count;


	SIM_device_t *devices;

	SIM_wire_t *wires;

}SIM_stage_t;

SIM_stage_t *SIM_init_stage(CFG_context_t *context);
static void SIM_init_stage_devices(SIM_stage_t *stage, CFG_context_t *context);



#endif