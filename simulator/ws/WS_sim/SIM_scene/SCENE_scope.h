#ifndef __SCENE_SCOPE_HEADER__
#define __SCENE_SCOPE_HEADER__

#include "SCENE_anchor.h"
#include <stdint.h>


typedef struct SCENE_scope
{
	bool filled;
	uint64_t devices_count;
	SCENE_device_t **devices;


}SCENE_scope_t;

//allocates a scope of devices
SCENE_scope_t *SCENE_init_scope( uint64_t count);

//add device ptr to scope
void SCENE_fill_scope(SCENE_scope_t *scope, SCENE_device_t **devices, uint64_t count);
//get device from scope
SCENE_device_t *SCENE_get_scope(SCENE_scope_t *scope, uint64_t index);
uint64_t SCENE_get_count_scope(SCENE_scope_t *scope);
void SCENE_print_scope(SCENE_scope_t *scope);



#endif