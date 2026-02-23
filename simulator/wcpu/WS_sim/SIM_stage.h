#ifndef __WS_SIM_STAGE_HEADER__
#define __WS_SIM_STAGE_HEADER__


#include "CFG_map.h"
#include "CFG_context.h"
#include "SIM_anchor.h"
#include "SIM_commons.h"
#include "SIM_wire.h"
#include "SIM_device.h"
#include <stdint.h>

typedef struct SIM_stage_map_element
{
	//the key of the device or wire
	int64_t key;
	//index in the device list
	uint64_t index;
}SIM_stage_map_elm_t;

typedef struct SIM_stage_map
{

	SIM_stage_map_elm_t *elms;
	uint64_t count;
}SIM_stage_map_t;

//stage needs to be redisgned to be a major holder
//stage -> scene -> graph
typedef struct SIM_stage
{
	//this is temporary and should be more complex
	bool has_generated;
	uint32_t devices_count;
	uint32_t wires_count;
	uint32_t anchors_count;
	
	SIM_device_t **devices;
	SIM_wire_t **wires;

	SIM_anchor_t **anchors;
	SIM_stage_map_t device_map;
	SIM_stage_map_t wire_map;
}SIM_stage_t;



typedef struct SIM_stage_generator
{
	CFG_context_t *context;
	SIM_stage_t *stage;
	uint64_t last_wire_size;
	uint64_t last_device_size;
}SIM_stage_gen_t;

SIM_stage_t *SIM_init_stage(void);


uint64_t SIM_get_stage_device_map(SIM_stage_t *stage, int64_t key);
uint64_t SIM_get_stage_wire_map(SIM_stage_t *stage, int64_t key);







//SIM fill stage, returns true

bool SIM_fill_stage(SIM_stage_t *stage, CFG_context_t *context);



#endif