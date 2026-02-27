#ifndef __SCENE_SCENE_HEADER__
#define __SCENE_SCENE_HEADER__



#include "SCENE_anchor.h"
#include "SIM_commons.h"
#include "SCENE_wire.h"
#include "SCENE_device.h"
#include "STAGE_stage.h"
#include <stdint.h>



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

}SCENE_scene_t;




SCENE_scene_t *SIM_init_stage(void);


uint64_t SIM_get_scene_device_map(SCENE_scene_t *stage, int64_t key);
uint64_t SIM_get_scene_wire_map(SCENE_scene_t *stage, int64_t key);







//SIM fill stage, returns true

bool SIM_fill_scene(SCENE_scene_t *scene, STAGE_stage_t *stage);




#endif
