

#include "SCENE_scene.h"


#include "CFG_map.h"
#include "CFG_link.h"
#include "SIM_commons.h"
#include "CFG_context.h"
#include "SCENE_anchor.h"
#include "SCENE_device.h"
#include "SCENE_wire.h"
#include "CFG_edge.h"
#include "commons.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>


//move all of this to scene


//stage helpers
uint64_t SCENE_get_stage_map(SCENE_scene_map_t *map, int64_t key);
void SCENE_free_stage_map(SCENE_scene_map_t *map);
bool SCENE_append_stage_map(SCENE_scene_map_t *map, int64_t key, uint64_t index);
void SCENE_build_stage_map(SCENE_scene_map_t *map, int64_t *keys, uint64_t *indexs, uint64_t size);


//generation helpers
void SCENE_define_stage_gen_anchors(SCENE_scene_gen_t *generator);
void SCENE_link_stage_gen_anchors(SCENE_scene_gen_t *generator);

//stage 1
//allocate the devices
void SCENE_alloc_stage_gen_devices(SCENE_scene_gen_t *generator);
void SCENE_alloc_stage_gen_wires(SCENE_scene_gen_t *generator);
//stage 2
//fill with locals
void SCENE_init_stage_gen_devices(SCENE_scene_gen_t *generator);
void SCENE_init_stage_gen_wires(SCENE_scene_gen_t *generator);

//stage 3
//fill with globals
void SCENE_resolve_stage_gen_devices(SCENE_scene_gen_t *generator);
void SCENE_resolve_stage_gen_wires(SCENE_scene_gen_t *generator);

//stage 4
//ugh... 4 is better than 3?
void SCENE_build_stage_gen_devices(SCENE_scene_gen_t *generator);
void SCENE_build_stage_gen_wire(SCENE_scene_gen_t *generator);

struct mapelm
{
	int64_t key;
	int64_t size;
};
struct mapelm *make_map(uint64_t size);
void append_map(struct mapelm *map, uint64_t size, int64_t key);
//generator stage
bool SCENE_check_conflicts_stage_gen(SCENE_scene_gen_t *gen);
void SCENE_rebuild_stage_gen(SCENE_scene_gen_t *gen);
void SCENE_alloc_stage_gen(SCENE_scene_gen_t *gen);
void SCENE_init_stage_gen(SCENE_scene_gen_t *gen);
void SCENE_resolve_stage_gen(SCENE_scene_gen_t *gen);
void SCENE_build_stage_gen(SCENE_scene_gen_t *gen);

//----------------------------------------
//
//				SCENE
//
//----------------------------------------

SCENE_scene_t *SCENE_init_stage(void)
{

	SCENE_scene_t *stage = calloc(1, sizeof(SCENE_scene_t));
	assert(stage);
	stage->has_generated = false;
	return stage;
}

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


//----------------------------------------
//
//				ANCHORS AND LINKING
//
//----------------------------------------
void SCENE_define_stage_gen_anchors(SCENE_scene_gen_t *generator)
{

	SCENE_scene_t *stage = generator->stage;
	CFG_context_t *context = generator->context;

	uint64_t count = context->wireconfigs->count;
	SCENE_anchor_t **anchors = calloc(count, sizeof(SCENE_anchor_t *));
	assert(anchors);
	for(uint64_t i = 0; i < count; ++i)
	{
		CFG_link_t *link = CFG_get_index_link_buf(context->channelbuf, i);
		SCENE_anchor_t *anc = SCENE_alloc_anchor(count);
		anchors[i] = anc;
	}
	stage->anchors = anchors;
	stage->anchors_count = count;
}


void SCENE_link_stage_gen_anchors(SCENE_scene_gen_t *generator)
{
	assert(0 && "link anchors");

}


//----------------------------------------
//
//				ALLOC STAGE
//
//----------------------------------------


void SCENE_alloc_stage_gen_devices(SCENE_scene_gen_t *generator)
{

	//asssuming check passed

	SCENE_scene_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
	uint64_t new_count = context->deviceconfigs->count;
	assert(new_count > 1 && "devices must be greater than 1");
	uint64_t last_count = stage->devices_count;

	uint64_t total_count = last_count + new_count;

	SCENE_device_t **devices = realloc_safe(stage->devices, total_count, sizeof(SCENE_device_t *));
	assert(devices);
	stage->devices = devices;
	stage->devices_count = total_count;
	generator->last_device_size = last_count;

	for(uint64_t i = last_count; i < total_count; ++i)
	{
		SCENE_device_t *dev = SCENE_alloc_device( );
		assert(dev != NULL && "device was null");

		stage->devices[i] = dev;

	}


}

void SCENE_alloc_stage_gen_wires(SCENE_scene_gen_t *generator)
{
	SCENE_scene_t *stage = generator->stage;
	CFG_context_t *context = generator->context;


	uint64_t new_count =  context->wireconfigs->count;
	uint64_t last_count = stage->wires_count;

	uint64_t total_count = last_count + new_count;


	SCENE_wire_t **wires = realloc_safe(stage->wires, total_count, sizeof(SCENE_wire_t *));

	assert(wires);
	stage->wires = wires;
	stage->wires_count = total_count;
	generator->last_wire_size = last_count;
	for(uint64_t i = last_count; i <  total_count; ++i)
	{
		SCENE_wire_t *wire = SCENE_alloc_wire();
		stage->wires[i] = wire;
	}

}



//----------------------------------------
//
//				INIT STAGE
//
//----------------------------------------

//TODO fix this later to be better
void SCENE_init_stage_gen_devices(SCENE_scene_gen_t *generator)
{
	SCENE_scene_t *stage = generator->stage;
	CFG_context_t *context = generator->context;

	for(uint64_t i = generator->last_device_size, j = 0; i < stage->devices_count; ++i, ++j)
	{
		SCENE_device_t *dev = stage->devices[i];
		SCENE_init_device(dev, &context->deviceconfigs->cfgs[j]);


	}

	for(uint64_t k = 0; k < stage->devices_count; ++k)
	{
		SCENE_device_t *dev = stage->devices[k];
		dev->id = k;
	}

}

//TODO fix this later to be better
void SCENE_init_stage_gen_wires(SCENE_scene_gen_t *generator)
{
	SCENE_scene_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
	for(uint64_t i = generator->last_wire_size, j = 0; i <  stage->wires_count; ++i, ++j)
	{
		SCENE_wire_t *wire = stage->wires[i];
		SCENE_init_wire(wire, &context->wireconfigs->cfgs[j]);
	}
}



//----------------------------------------
//
//				RESOLVE STAGE
//
//----------------------------------------

//stage 3
void SCENE_resolve_stage_gen_devices(SCENE_scene_gen_t *generator)
{
	SCENE_scene_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
}

void SCENE_resolve_stage_gen_wires(SCENE_scene_gen_t *generator)
{
	SCENE_scene_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
}



//----------------------------------------
//
//				BUILD STAGE
//
//----------------------------------------

void SCENE_build_stage_gen_devices(SCENE_scene_gen_t *generator)
{
	SCENE_scene_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
}

void SCENE_build_stage_gen_wire(SCENE_scene_gen_t *generator)
{
	SCENE_scene_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
}

//----------------------------------------
//
//				GENERATOR STAGE
//
//----------------------------------------


bool SCENE_fill_stage(SCENE_scene_t *stage, CFG_context_t *context)
{
	if(stage->has_generated)
	{
		assert(0 && "SCENEulator has generated");
		return false;
	}

	SCENE_scene_gen_t gen =
	{
		.stage = stage,
		.context = context
	};

	bool passed = SCENE_check_conflicts_stage_gen(&gen);
	if(passed == false)
	{
		assert("could not fill stage");
	}
	//allocate all the parts
	SCENE_alloc_stage_gen(&gen);
	//preset anchors space
	//fill all inits
	SCENE_init_stage_gen(&gen);
	SCENE_resolve_stage_gen(&gen);
	SCENE_build_stage_gen(&gen);
	//SCENE generator should support transitions this is not done yet
	printf("SCENE generator should support transitions\nthis is not done yet\n");
	stage->has_generated = true;
	return true;
}

struct mapelm *make_map(uint64_t size)
{
	struct mapelm *overlap = calloc(size, sizeof(struct mapelm));
	for(uint64_t s = 0; s < size; ++s)
	{
		overlap[s].key = -1;
	}
	return overlap;
}

void append_map(struct mapelm *map, uint64_t size, int64_t key)
{
	uint64_t ukey = (uint64_t)key;
	uint64_t pos = ukey % size;


	for(uint64_t i = 0; i < size; ++i)
	{
		if(map[pos].key == key)
		{
			map[pos].size++;
			return;
		}
		else if(map[pos].key == -1)
		{
			map[pos].key = key;
			map[pos].size = 1;
			return;
		}
		else
		{
			pos = (pos + 1) % size;
		}


	}
	assert(0 && "some how failed");
}

bool SCENE_check_conflicts_stage_gen(SCENE_scene_gen_t *gen)
{
	SCENE_scene_t *stage = gen->stage;
	CFG_context_t *context = gen->context;

	const uint64_t cur_dev_count = stage->devices_count;
	const uint64_t add_dev_count = context->deviceconfigs->count;
	const uint64_t minimum_size1 = cur_dev_count + add_dev_count;


	struct mapelm *map1 = make_map(minimum_size1);

	for(uint64_t i1 = 0; i1 < cur_dev_count; ++i1)
	{
		int64_t tag = gen->stage->devices[i1]->dkey;
		append_map(map1, minimum_size1, tag);
	}

	for(uint64_t i2 = 0; i2 < add_dev_count; ++i2)
	{
		int64_t tag = context->deviceconfigs->cfgs[i2].pretag;
		append_map(map1, minimum_size1, tag);
	}

	bool passed1 = true;
	for(uint64_t ic1 = 0; ic1 < minimum_size1; ++ic1)
	{
		if(map1[ic1].size > 1)
		{
			passed1 = false;
			break;
		}
	}

	free(map1);

	const uint64_t cur_wire_count = stage->wires_count;
	const uint64_t add_wire_count = context->wireconfigs->count;
	const uint64_t minimum_size2 = cur_wire_count + add_wire_count;


	struct mapelm *map2 = make_map(minimum_size2);

	for(uint64_t i3 = 0; i3 < cur_wire_count; ++i3)
	{
		int64_t tag = stage->wires[i3]->wkey;
		append_map(map2, minimum_size2, tag);
	}

	for(uint64_t i4 = 0; i4 < add_wire_count; ++i4)
	{
		int64_t tag = context->wireconfigs->cfgs[i4].id;
		append_map(map2, minimum_size2, tag);
	}

	bool passed2 = true;
	for(uint64_t ic2 = 0; ic2 < minimum_size2; ++ic2)
	{

		if(map2[ic2].size > 1)
		{
			passed2 = false;
			break;
		}
	}

	free(map2);

	return passed1 && passed2;

}

void SCENE_alloc_stage_gen(SCENE_scene_gen_t *gen)
{
	SCENE_alloc_stage_gen_devices(gen);
	SCENE_alloc_stage_gen_wires(gen);
	SCENE_define_stage_gen_anchors(gen);

}

void SCENE_init_stage_gen(SCENE_scene_gen_t *gen)
{
	SCENE_init_stage_gen_devices(gen);
	SCENE_init_stage_gen_wires(gen);
}

void SCENE_resolve_stage_gen(SCENE_scene_gen_t *gen)
{
	SCENE_rebuild_stage_gen(gen);
	SCENE_link_stage_gen_anchors(gen);


}

void SCENE_build_stage_gen(SCENE_scene_gen_t *gen)
{

}

void SCENE_rebuild_stage_gen(SCENE_scene_gen_t *gen)
{
    SCENE_scene_t *stage = gen->stage;

    uint64_t ndev = stage->devices_count;
    uint64_t nwire = stage->wires_count;

    int64_t *dkeys = malloc(sizeof(int64_t)*ndev);
    uint64_t *didx = malloc(sizeof(uint64_t)*ndev);

    for(uint64_t i = 0; i < ndev; i++)
	{
        dkeys[i] = stage->devices[i]->dkey;
        didx[i]  = i;
    }

    SCENE_build_stage_map(&stage->device_map, dkeys, didx, ndev);

    free(dkeys);
    free(didx);

    int64_t *wkeys = malloc(sizeof(int64_t) * nwire);
    uint64_t *widx = malloc(sizeof(uint64_t) * nwire);

    for(uint64_t i = 0; i < nwire; i++)
	{
        wkeys[i] = stage->wires[i]->wkey;
        widx[i]  = i;
    }

    SCENE_build_stage_map(&stage->wire_map, wkeys, widx, nwire);

    free(wkeys);
    free(widx);
}
