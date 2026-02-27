
#include "SCENE_scene.h"


#include "SIM_commons.h"
#include "SCENE_anchor.h"
#include "SCENE_device.h"
#include "SCENE_wire.h"
#include "commons.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>


//move all of this to scene

/*


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


*/