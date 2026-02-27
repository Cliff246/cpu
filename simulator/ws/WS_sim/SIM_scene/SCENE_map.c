#include "SCENE_map.h"

/*

uint64_t SCENE_get_stage_map(SCENE_scene_map_t *map, int64_t key)
{
	uint64_t ukey = (uint64_t)key;
	uint64_t pos = ukey % map->count;


	for(uint64_t i = 0; i < map->count; ++i)
	{
		if(map->elms[pos].key == key)
		{
			return map->elms[pos].index;
		}
		else
		{
			pos = (pos + 1) % map->count;
		}
	}
	assert(0 && "failed to get key");
	return 0;
}

void SCENE_free_stage_map(SCENE_scene_map_t *map)
{
	free(map->elms);
	map->elms = NULL;
	map->count = 0;
}

bool SCENE_append_stage_map(SCENE_scene_map_t *map, int64_t key, uint64_t index)
{

	uint64_t ukey = (uint64_t)key;
	uint64_t pos = ukey % map->count;


	for(uint64_t i = 0; i < map->count; ++i)
	{
		if(map->elms[pos].key == key)
		{
			return false;
		}
		if(map->elms[pos].key == -1)
		{
			map->elms[pos].key = key;
			map->elms[pos].index = index;
			return true;
		}
		else
		{
			pos = (pos + 1) % map->count;
		}


	}
	assert(0 && "failed to get key");
	return false;
}

void SCENE_build_stage_map(SCENE_scene_map_t *map, int64_t *keys, uint64_t *indexs, uint64_t size)
{
	SCENE_free_stage_map(map);
	assert(size > 0);
	assert(keys);
	assert(indexs);
	SCENE_scene_map_elm_t *elms =  calloc(size, sizeof(SCENE_scene_map_elm_t));
	assert(elms);
	map->elms = elms;
	map->count = size;



	for(uint64_t i = 0; i < size; ++i)
	{
		elms[i].key = -1;
	}

	for(uint64_t j = 0; j < size; ++j)
	{
		bool passes = SCENE_append_stage_map(map, keys[j], indexs[j]);
		assert(passes == true);
	}

}

//stage helpers
uint64_t SCENE_get_stage_device_map(SCENE_scene_t *stage, int64_t key)
{
	return SCENE_get_stage_map(&stage->device_map, key);
}

uint64_t SCENE_get_stage_wire_map(SCENE_scene_t *stage, int64_t key)
{
	return SCENE_get_stage_map(&stage->wire_map, key);

}

*/