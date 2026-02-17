#include "SIM_stage.h"


#include "CFG_map.h"
#include "OBJ_bundle.h"
#include "CFG_link.h"
#include "RUN_graph.h"
#include "SIM_anchor.h"
#include "SIM_commons.h"
#include "CFG_context.h"
#include "SIM_device.h"
#include "CFG_edge.h"
#include "SIM_wire.h"
#include "commons.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>


//stage helpers
uint64_t SIM_get_stage_map(SIM_stage_map_t *map, int64_t key);
void SIM_free_stage_map(SIM_stage_map_t *map);
bool SIM_append_stage_map(SIM_stage_map_t *map, int64_t key, uint64_t index);
void SIM_build_stage_map(SIM_stage_map_t *map, int64_t *keys, uint64_t *indexs, uint64_t size);


//generation helpers
void SIM_define_stage_gen_anchors(SIM_stage_gen_t *generator);
void SIM_link_stage_gen_anchors(SIM_stage_gen_t *generator);

//stage 1
//allocate the devices
void SIM_alloc_stage_gen_devices(SIM_stage_gen_t *generator);
void SIM_alloc_stage_gen_wires(SIM_stage_gen_t *generator);
//stage 2
//fill with locals
void SIM_init_stage_gen_devices(SIM_stage_gen_t *generator);
void SIM_init_stage_gen_wires(SIM_stage_gen_t *generator);

//stage 3
//fill with globals
void SIM_resolve_stage_gen_devices(SIM_stage_gen_t *generator);
void SIM_resolve_stage_gen_wires(SIM_stage_gen_t *generator);
//stage 4
//ugh... 4 is better than 3?
void SIM_build_stage_gen_devices(SIM_stage_gen_t *generator);
void SIM_build_stage_gen_wire(SIM_stage_gen_t *generator);

struct mapelm
{
	int64_t key;
	int64_t size;
};
struct mapelm *make_map(uint64_t size);
void append_map(struct mapelm *map, uint64_t size, int64_t key);
//generator stage
bool SIM_check_conflicts_stage_gen(SIM_stage_gen_t *gen);
void SIM_rebuild_stage_gen(SIM_stage_gen_t *gen);
void SIM_alloc_stage_gen(SIM_stage_gen_t *gen);
void SIM_init_stage_gen(SIM_stage_gen_t *gen);
void SIM_resolve_stage_gen(SIM_stage_gen_t *gen);
void SIM_build_stage_gen(SIM_stage_gen_t *gen);

//----------------------------------------
//
//				SIM
//
//----------------------------------------

SIM_stage_t *SIM_init_stage(void)
{

	SIM_stage_t *stage = calloc(1, sizeof(SIM_stage_t));
	assert(stage);
	stage->has_generated = false;
	return stage;
}

uint64_t SIM_get_stage_map(SIM_stage_map_t *map, int64_t key)
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

void SIM_free_stage_map(SIM_stage_map_t *map)
{
	free(map->elms);
	map->elms = NULL;
	map->count = 0;
}

bool SIM_append_stage_map(SIM_stage_map_t *map, int64_t key, uint64_t index)
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

void SIM_build_stage_map(SIM_stage_map_t *map, int64_t *keys, uint64_t *indexs, uint64_t size)
{
	SIM_free_stage_map(map);
	assert(size > 0);
	assert(keys);
	assert(indexs);
	SIM_stage_map_elm_t *elms =  calloc(size, sizeof(SIM_stage_map_elm_t));
	assert(elms);
	map->elms = elms;
	map->count = size;



	for(uint64_t i = 0; i < size; ++i)
	{
		elms[i].key = -1;
	}

	for(uint64_t j = 0; j < size; ++j)
	{
		bool passes = SIM_append_stage_map(map, keys[j], indexs[j]);
		assert(passes == true);
	}

}

//stage helpers
uint64_t SIM_get_stage_device_map(SIM_stage_t *stage, int64_t key)
{
	return SIM_get_stage_map(&stage->device_map, key);
}

uint64_t SIM_get_stage_wire_map(SIM_stage_t *stage, int64_t key)
{
	return SIM_get_stage_map(&stage->wire_map, key);

}


//----------------------------------------
//
//				ANCHORS AND LINKING
//
//----------------------------------------
void SIM_define_stage_gen_anchors(SIM_stage_gen_t *generator)
{

	SIM_stage_t *stage = generator->stage;
	CFG_context_t *context = generator->context;

	uint64_t count = context->wireconfigs->count;
	SIM_anchor_t **anchors = calloc(count, sizeof(SIM_anchor_t *));
	assert(anchors);
	for(uint64_t i = 0; i < count; ++i)
	{
		CFG_link_t *link = CFG_get_index_link_buf(context->channelbuf, i);
		SIM_anchor_t *anc = SIM_alloc_anchor(count);
		anchors[i] = anc;
	}
	stage->anchors = anchors;
	stage->anchors_count = count;
}


void SIM_link_stage_gen_anchors(SIM_stage_gen_t *generator)
{
	assert(0 && "link anchors");

}


//----------------------------------------
//
//				ALLOC STAGE
//
//----------------------------------------


void SIM_alloc_stage_gen_devices(SIM_stage_gen_t *generator)
{

	//asssuming check passed

	SIM_stage_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
	uint64_t new_count = context->deviceconfigs->count;
	assert(new_count > 1 && "devices must be greater than 1");
	uint64_t last_count = stage->devices_count;

	uint64_t total_count = last_count + new_count;

	SIM_device_t **devices = realloc_safe(stage->devices, total_count, sizeof(SIM_device_t *));
	assert(devices);
	stage->devices = devices;
	stage->devices_count = total_count;
	generator->last_device_size = last_count;

	for(uint64_t i = last_count; i < total_count; ++i)
	{
		SIM_device_t *dev = SIM_alloc_device( );
		assert(dev != NULL && "device was null");

		stage->devices[i] = dev;

	}


}

void SIM_alloc_stage_gen_wires(SIM_stage_gen_t *generator)
{
	SIM_stage_t *stage = generator->stage;
	CFG_context_t *context = generator->context;


	uint64_t new_count =  context->wireconfigs->count;
	uint64_t last_count = stage->wires_count;

	uint64_t total_count = last_count + new_count;


	SIM_wire_t **wires = realloc_safe(stage->wires, total_count, sizeof(SIM_wire_t *));

	assert(wires);
	stage->wires = wires;
	stage->wires_count = total_count;
	generator->last_wire_size = last_count;
	for(uint64_t i = last_count; i <  total_count; ++i)
	{
		SIM_wire_t *wire = SIM_alloc_wire();
		stage->wires[i] = wire;
	}

}



//----------------------------------------
//
//				INIT STAGE
//
//----------------------------------------

//TODO fix this later to be better
void SIM_init_stage_gen_devices(SIM_stage_gen_t *generator)
{
	SIM_stage_t *stage = generator->stage;
	CFG_context_t *context = generator->context;

	for(uint64_t i = generator->last_device_size, j = 0; i < stage->devices_count; ++i, ++j)
	{
		SIM_device_t *dev = stage->devices[i];
		SIM_init_device(dev, &context->deviceconfigs->cfgs[j]);


	}

	for(uint64_t k = 0; k < stage->devices_count; ++k)
	{
		SIM_device_t *dev = stage->devices[k];
		dev->id = k;
	}

}

//TODO fix this later to be better
void SIM_init_stage_gen_wires(SIM_stage_gen_t *generator)
{
	SIM_stage_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
	for(uint64_t i = generator->last_wire_size, j = 0; i <  stage->wires_count; ++i, ++j)
	{
		SIM_wire_t *wire = stage->wires[i];
		SIM_init_wire(wire, &context->wireconfigs->cfgs[j]);
	}
}



//----------------------------------------
//
//				RESOLVE STAGE
//
//----------------------------------------

//stage 3
void SIM_resolve_stage_gen_devices(SIM_stage_gen_t *generator)
{
	SIM_stage_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
}

void SIM_resolve_stage_gen_wires(SIM_stage_gen_t *generator)
{
	SIM_stage_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
}



//----------------------------------------
//
//				BUILD STAGE
//
//----------------------------------------

void SIM_build_stage_gen_devices(SIM_stage_gen_t *generator)
{
	SIM_stage_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
}

void SIM_build_stage_gen_wire(SIM_stage_gen_t *generator)
{
	SIM_stage_t *stage = generator->stage;
	CFG_context_t *context = generator->context;
}

//----------------------------------------
//
//				GENERATOR STAGE
//
//----------------------------------------


bool SIM_fill_stage(SIM_stage_t *stage, CFG_context_t *context)
{
	if(stage->has_generated)
	{
		assert(0 && "simulator has generated");
		return false;
	}

	SIM_stage_gen_t gen =
	{
		.stage = stage,
		.context = context
	};

	bool passed = SIM_check_conflicts_stage_gen(&gen);
	if(passed == false)
	{
		assert("could not fill stage");
	}
	//allocate all the parts
	SIM_alloc_stage_gen(&gen);
	//preset anchors space
	//fill all inits
	SIM_init_stage_gen(&gen);
	SIM_resolve_stage_gen(&gen);
	SIM_build_stage_gen(&gen);
	//SIM generator should support transitions this is not done yet
	printf("SIM generator should support transitions\nthis is not done yet\n");
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

bool SIM_check_conflicts_stage_gen(SIM_stage_gen_t *gen)
{
	SIM_stage_t *stage = gen->stage;
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

void SIM_alloc_stage_gen(SIM_stage_gen_t *gen)
{
	SIM_alloc_stage_gen_devices(gen);
	SIM_alloc_stage_gen_wires(gen);
	SIM_define_stage_gen_anchors(gen);

}

void SIM_init_stage_gen(SIM_stage_gen_t *gen)
{
	SIM_init_stage_gen_devices(gen);
	SIM_init_stage_gen_wires(gen);
}

void SIM_resolve_stage_gen(SIM_stage_gen_t *gen)
{
	SIM_rebuild_stage_gen(gen);
	SIM_link_stage_gen_anchors(gen);


}

void SIM_build_stage_gen(SIM_stage_gen_t *gen)
{

}

void SIM_rebuild_stage_gen(SIM_stage_gen_t *gen)
{
    SIM_stage_t *stage = gen->stage;

    uint64_t ndev = stage->devices_count;
    uint64_t nwire = stage->wires_count;

    int64_t *dkeys = malloc(sizeof(int64_t)*ndev);
    uint64_t *didx = malloc(sizeof(uint64_t)*ndev);

    for(uint64_t i = 0; i < ndev; i++)
	{
        dkeys[i] = stage->devices[i]->dkey;
        didx[i]  = i;
    }

    SIM_build_stage_map(&stage->device_map, dkeys, didx, ndev);

    free(dkeys);
    free(didx);

    int64_t *wkeys = malloc(sizeof(int64_t) * nwire);
    uint64_t *widx = malloc(sizeof(uint64_t) * nwire);

    for(uint64_t i = 0; i < nwire; i++)
	{
        wkeys[i] = stage->wires[i]->wkey;
        widx[i]  = i;
    }

    SIM_build_stage_map(&stage->wire_map, wkeys, widx, nwire);

    free(wkeys);
    free(widx);
}
