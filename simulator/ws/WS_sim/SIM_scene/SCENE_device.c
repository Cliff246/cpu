#include "SCENE_device.h"
#include "CFG_link.h"
#include "SIM_handle.h"
#include "TAG_int.h"
#include "TAG_list.h"
#include "dynamic_lib.h"
#include "hashmap.h"

#include "TAG_tag.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>



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
