#ifndef __SCENE_MAP_HEADER__
#define __SCENE_MAP_HEADER__

#include <stdint.h>

typedef struct SCENE_map_element
{
	//the key of the device or wire
	int64_t key;
	//index in the device list
	uint64_t index;
}SCENE_mapelm_t;

typedef struct SCENE_map
{

	SCENE_mapelm_t *elms;
	uint64_t count;
}SCENE_map_t;

#endif