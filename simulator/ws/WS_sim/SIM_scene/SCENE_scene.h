#ifndef __SCENE_SCENE_HEADER__
#define __SCENE_SCENE_HEADER__

#include "CFG_map.h"
#include "CFG_context.h"
#include "SCENE_anchor.h"
#include "SIM_commons.h"
#include "SCENE_wire.h"
#include "SCENE_device.h"
#include <stdint.h>

typedef struct SCENE_scene_map_element
{
	//the key of the device or wire
	int64_t key;
	//index in the device list
	uint64_t index;
}SCENE_scene_map_elm_t;

typedef struct SCENE_scene_map
{

	SCENE_scene_map_elm_t *elms;
	uint64_t count;
}SCENE_scene_map_t;

//stage needs to be redisgned to be a major holder
//stage -> scene -> graph
typedef struct SCENE_scene
{
	//this is temporary and should be more complex
	bool has_generated;
	uint32_t devices_count;
	uint32_t wires_count;
	uint32_t anchors_count;
	
	SCENE_device_t **devices;
	SCENE_wire_t **wires;

	SCENE_anchor_t **anchors;
	SCENE_scene_map_t device_map;
	SCENE_scene_map_t wire_map;
}SCENE_scene_t;



typedef struct SCENE_scene_generator
{
	CFG_context_t *context;
	SCENE_scene_t *stage;
	uint64_t last_wire_size;
	uint64_t last_device_size;
}SCENE_scene_gen_t;

SCENE_scene_t *SIM_init_stage(void);


uint64_t SIM_get_stage_device_map(SCENE_scene_t *stage, int64_t key);
uint64_t SIM_get_stage_wire_map(SCENE_scene_t *stage, int64_t key);







//SIM fill stage, returns true

bool SIM_fill_stage(SCENE_scene_t *stage, CFG_context_t *context);




#endif
