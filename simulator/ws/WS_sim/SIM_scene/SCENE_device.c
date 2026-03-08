#include "SCENE_device.h"
#include "SCENE_anchor.h"
#include "SIM_handle.h"
#include "STAGE_actor.h"
#include "TAG_int.h"
#include "TAG_list.h"
#include "TAG_map.h"
#include "TAG_string.h"
#include "dynamic_lib.h"
#include "hashmap.h"
#include "SCENE_keywords.h"
#include "TAG_tag.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


//-----------------------------------------
//
//	device commons
//
//----------------------------------------



//---------------------
//DEVICE commons functions
//---------------------



TAG_tag_t *has_tag(TAG_tag_t *base, char *key);



int64_t get_mono_map(TAG_tag_t *base);
uint64_t get_keywords_size(char **string);


//-----------------------------------------
//
//	device locals
//
//----------------------------------------

//---------------------
//DEVICE validation locals
//---------------------







//----------------------
//device validation functions
//----------------------
int SCENE_validate_device_strings(SCENE_device_t *device, char **strings, uint64_t size, bool *buffer);






//commons


TAG_tag_t *has_tag(TAG_tag_t *base, char *key)
{
	TAG_argptr_t get_key = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_KEY_STRING);
	TAG_tag_t *contains = get_key.MAP->get_key_string(base, key);
	return contains;
}


uint64_t get_keywords_size(char **string)
{
	uint64_t i = 0;

	while(string[i] != NULL)
	{
		++i;
	}
	return i;
}


//SCENE validate

int SCENE_validate_device_strings(SCENE_device_t *device, char **strings, uint64_t size, bool *buffer)
{
	TAG_tag_t *base = device->actor->tags;
	TAG_argptr_t get_key = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_KEY_STRING);
	TAG_argptr_t rem_key = TAG_get_fn(TAG_MAP, TAG_FN_MAP_REM_KEY_STRING);
	TAG_argptr_t start_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_START);
	TAG_argptr_t reset_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_RESET);
	TAG_argptr_t up_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_UP);
	TAG_argptr_t is_end_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_IS_END);
	TAG_argptr_t end_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_END);
	TAG_argptr_t is_int_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_IS_KEY_INT);
	TAG_argptr_t is_str_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_IS_KEY_STR);
	TAG_argptr_t get_str_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_GET_KEY_STR);
	TAG_argptr_t get_int_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_GET_KEY_INT);
	TAG_argptr_t get_size = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_SIZE);

	TAG_tag_t *copy = TAG_copy(base);
	bool passed = true;
	for(uint64_t i = 0; i < size; ++i)
	{
		char *key = strings[i];
		TAG_tag_t *contains = get_key.MAP->get_key_string(base, key);
		bool test = (contains)? true : false;
		assert(rem_key.MAP->rem_key_string(copy, key));
		buffer[i] = test;
		passed = passed && test;
	}
	//printf("copy\n");
	//TAG_map_print_entries(copy->ptr);
	//TAG_print(copy);

	uint64_t excess = get_size.MAP->get_size(copy);
	if(excess > 0)
	{

		start_iter.MAP->iter_start(copy);
		reset_iter.MAP->iter_reset(copy);
		while(is_end_iter.MAP->iter_is_end(copy) == false)
		{
			bool is_str = is_str_iter.MAP->iter_is_key_str(copy);
			if(is_str)
			{
				char *strkey = get_str_iter.MAP->iter_get_key_str(copy);
				printf("extra key %s\n", strkey);
			}

			up_iter.MAP->iter_up(copy);
		}
		end_iter.MAP->iter_end(copy);
		assert(0);
	}

	for(uint64_t i = 0; i < size; ++i)
	{
		printf("[%s] = %d\n", strings[i], buffer[i]);
	}
	printf("\n");
	TAG_free(copy);
	return excess;

}





//device globals

SCENE_device_t *SCENE_alloc_device(STAGE_actor_t *actor)
{

	SCENE_device_t *device = calloc(1, sizeof(SCENE_device_t));

	device->actor = actor;


	return device;

}


/*
	TAG_tag_t *base = device->actor->tags;
	assert(base->type == TAG_MAP);
	TAG_argptr_t get_key = TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_KEY_STRING);
	TAG_argptr_t get_string = TAG_get_fn(TAG_STRING, TAG_FN_STRING_GET);
	TAG_tag_t *key = get_key.MAP->get_key_string(base, "module");
	assert(key->type == TAG_MAP);
	TAG_argptr_t map_reset_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_RESET);
	TAG_argptr_t map_up_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_UP);
	TAG_argptr_t map_end_iter = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_END);
	TAG_argptr_t map_get_value = TAG_get_fn(TAG_MAP, TAG_FN_MAP_ITER_GET_VALUE);
	map_reset_iter.MAP->iter_reset(key);
	while(true)
	{
		bool end = map_end_iter.MAP->iter_is_end(key);
		if(end == true)
			break;
		TAG_tag_t *tag = map_get_value.MAP->iter_get_value(key);
		if(tag->type == TAG_STRING)
		{
			char *string = get_string.STRING->get(tag);
			printf("string:%s\n", string);
			SIM_handle_t *handle = SIM_init_handle(string);
			for(uint64_t i = 0; i < handle->desc->symmap->size; ++i)
			{
				handle->desc->symmap->symbols[i].hook->OBJ_hook_fn(NULL, NULL, NULL, NULL, NULL);
			}
		}
		map_up_iter.MAP->iter_up(key);
	}
	map_reset_iter.MAP->iter_reset(key);
*/

bool SCENE_validate_device(SCENE_device_t *device)
{

	const uint64_t count_keywords = sizeof(keywords_strings) / sizeof(keywords_strings[0]);

	bool buffer[count_keywords];

	int excess = SCENE_validate_device_strings(device, keywords_strings, count_keywords, buffer);
	printf("excess %d\n", excess);

	return false;
}

void SCENE_print_device(SCENE_device_t *device)
{
	//STAGE_print_actor(device->actor);
}

/*

static p_hashtable_t SCENE_copy_hashtable(p_hashtable_t old)
{
 	p_hashtable_t copy = new_hash_table(100, SIM_free_tag_table_elem);


	reset_iter_hashtable(old);
	p_hashelem_t elem = next_iter_hashtable(old);
	while(elem != NULL)
	{
		TAG_tag_t *tagcopy = TAG_copy(elem->p_data);
		//TAG_print(tagcopy);
		addto_hash_table(copy, elem->p_key, tagcopy);
		elem = next_iter_hashtable(old);

	}
	return copy;
}

//allocate device
SCENE_device_t *SCENE_alloc_device()
{

	SCENE_device_t *device = calloc(1, sizeof(SCENE_device_t));
	assert(device);

	return device;

}



//init
void SCENE_init_device(SCENE_device_t *device, CFG_node_t *devctx)
{

	device->tags = SCENE_copy_hashtable(devctx->initals);
	device->handle = SIM_init_handle(devctx->module);
	device->dkey = (int64_t)devctx->pretag;

}


void SCENE_resolve_device(SCENE_device_t *device)
{
	assert(0 && "TODO");
}

void SCENE_build_device(SCENE_device_t *device)
{
	assert(0 && "TODO");

}


void SCENE_device_print(SCENE_device_t *device)
{

}


void SCENE_free_device(SCENE_device_t *device)
{
	assert(0 && "lol imagine freeing this ");
}


*/