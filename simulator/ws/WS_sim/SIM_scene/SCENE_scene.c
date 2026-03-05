
#include "SCENE_scene.h"


#include "SCENE_context.h"
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




//SCENE locals
/*
struct mapelm
{
	int64_t key;
	int64_t size;
};
struct mapelm *make_map(uint64_t size);
void append_map(struct mapelm *map, uint64_t size, int64_t key);
//generator stage
void SCENE_rebuild_stage_gen(SCENE_scene_t *gen);
void SCENE_alloc_stage_gen(SCENE_scene_t *gen);
void SCENE_init_stage_gen(SCENE_scene_t *gen);
void SCENE_resolve_stage_gen(SCENE_scene_t *gen);
void SCENE_build_stage_gen(SCENE_scene_t *gen);
*/


bool SCENE_check_conflicts_scene(SCENE_scene_t *scene);


//----------------------------------------
//
//				SCENE
//
//----------------------------------------

bool SCENE_check_conflicts_scene(SCENE_scene_t *scene)
{
	return false;
}




//move all of this to scene

/*

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

SCENE_scene_t *SCENE_init_scene(void)
{
	SCENE_scene_t *scene = calloc(1, sizeof(SCENE_scene_t));
	scene->filled = false;
	return scene;
}


bool SCENE_fill_scene(SCENE_scene_t *scene, STAGE_stage_t *stage)
{
	if(scene->filled == false)
	{
		scene->stage = stage;
		scene->filled = true;
		return true;
	}
	assert(0 && "cannot fill existing scene");
	return false;

}

bool SCENE_generate_scene(SCENE_scene_t *scene)
{
	assert(scene->filled && "scene must be filled");
	SCENE_context_t *context = SCENE_alloc_context(scene);
	scene->context = context;
	SCENE_init_context(context);



	return false;
}

bool SCENE_teardown_scene(SCENE_scene_t *scene)
{
	assert(scene->filled && "scene must be filled");
	if(scene->filled == false)
	{
		return false;
	}
	return true;
}

void SCENE_free_scene(SCENE_scene_t *scene)
{
	assert(!scene->filled && "scene must not be filled");

	if(scene->filled == true)
	{
		assert(0 && "todo error out that scene is free'd");
	}

	assert(0 && "todo free scene");
	free(scene);
}
