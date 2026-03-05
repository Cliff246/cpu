#ifndef __WS_SCENE_DEVICE_HEADER__
#define __WS_SCENE_DEVICE_HEADER__

#include "MOD_description.h"
#include "RES_interface.h"
#include "RES_pool.h"
#include "SCENE_anchor.h"
#include "SIM_commons.h"
#include "SIM_handle.h"
#include "STAGE_actor.h"
#include "dynamic_lib.h"
#include "hashmap.h"
#include "SCENE_port.h"

#include <stdint.h>
#include <stdbool.h>



typedef struct SCENE_device
{

	STAGE_actor_t *actor;
	SIM_uid_t uid;
	char *name;


}SCENE_device_t;



SCENE_device_t *SCENE_alloc_device(STAGE_actor_t *actor);
bool SCENE_validate_device(SCENE_device_t *device);

void SCENE_print_device(SCENE_device_t *device);

/*
//locals to the device
static p_hashtable_t SCENE_copy_hashtable(p_hashtable_t old);


//init stage

SCENE_device_t *SCENE_alloc_device();
void SCENE_init_device(SCENE_device_t *device, CFG_node_t *devctx);
void SCENE_resolve_device(SCENE_device_t *device);
void SCENE_build_device(SCENE_device_t *device);


//free(not done yet)
void SCENE_free_device(SCENE_device_t *device);



void SCENE_device_print(SCENE_device_t *device);

*/

#endif
