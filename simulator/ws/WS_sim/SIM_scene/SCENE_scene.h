#ifndef __SCENE_SCENE_HEADER__
#define __SCENE_SCENE_HEADER__



#include "SCENE_context.h"
#include "SIM_commons.h"
#include "SCENE_device.h"
#include "STAGE_stage.h"
#include <stdint.h>




//stage needs to be redisgned to be a major holder
//stage -> scene -> graph
typedef struct SCENE_scene
{
	//this is temporary and should be more complex
	STAGE_stage_t *stage;
	SCENE_context_t *context;

	bool filled;

}SCENE_scene_t;


//init a scene ptr
SCENE_scene_t *SCENE_init_scene(void);
//fill a empty scene with a unattached stage
bool SCENE_fill_scene(SCENE_scene_t *scene, STAGE_stage_t *stage);
//generates scene via fill
bool SCENE_generate_scene(SCENE_scene_t *scene);
//tears down generated scene
bool SCENE_teardown_scene(SCENE_scene_t *scene);


void SCENE_free_scene(SCENE_scene_t *scene);


//SIM fill stage, returns true





#endif
