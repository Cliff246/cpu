#include "SCENE_context.h"
#include "SCENE_anchor.h"
#include "SCENE_device.h"
#include "SCENE_scene.h"
#include "SCENE_scope.h"
#include "SCENE_topology.h"
#include "STAGE_stage.h"



#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



//step 0
//scene allocate all devices and copy over basics
bool SCENE_alloc_step_context(SCENE_context_t *context);

//step 1


//step 2
//scene validate structures
bool SCENE_validate_step_context(SCENE_context_t *context);


//step 3
//scene assign
bool SCENE_assign_step_context(SCENE_context_t *context);



//stage 4
//scene symbolize
bool SCENE_symbolize_step_context(SCENE_context_t *context);

//stage 3
//fill with globals


//stage 4
//ugh... 4 is better than 3?

bool (*scene_steps[])(SCENE_context_t *context) =
{
	[SCENE_CONTEXT_STEP_START] = NULL,
	[SCENE_CONTEXT_STEP_ALLOC] = SCENE_alloc_step_context,
	[SCENE_CONTEXT_STEP_VALIDATE] = SCENE_validate_step_context,
	[SCENE_CONTEXT_STEP_ASSIGN] = SCENE_assign_step_context,
	[SCENE_CONTEXT_STEP_SYMBOLIZE] = SCENE_symbolize_step_context,
};

#define SCENE_CONTEXT_STEP_LIST_STRING(X) [SCENE_CONTEXT_STEP_LIST_NAME(X)] = #X,
//context step strings
char *scene_steps_names[] =
{
	SCENE_CONTEXT_STEP_LIST(SCENE_CONTEXT_STEP_LIST_STRING)
};

//----------------------------------------
//
//				SCENE ALLOC
//
//----------------------------------------

bool SCENE_alloc_step_context(SCENE_context_t *context)
{
	STAGE_stage_t *stage = context->scene->stage;
	uint64_t size = stage->actors_size;

	SCENE_device_t *devices[size];
	//allocate devices
	for(uint64_t i = 0; i < size; ++i)
	{
		SCENE_device_t *device = SCENE_alloc_device(stage->actors[i]);
		device->uid = i;
		devices[i] = device;
	}

	SCENE_scope_t *scope = SCENE_init_scope(size);
	SCENE_fill_scope(scope, devices, size);
	context->scope = scope;
	//SCENE_print_scope(context->scope);

	return true;
}

//----------------------------------------
//
//				SCENE VALIDATE
//
//----------------------------------------


bool SCENE_validate_step_context(SCENE_context_t *context)
{
	uint64_t size = SCENE_get_count_scope(context->scope);

	for(uint64_t i = 0; i < size; ++i)
	{
		SCENE_device_t *device = SCENE_get_scope(context->scope, i);

		assert(SCENE_validate_device(device) == true);

	}

	return true;
}


//----------------------------------------
//
//				SCENE ASSIGN
//
//----------------------------------------



bool SCENE_assign_step_context(SCENE_context_t *context)
{
	uint64_t size = SCENE_get_count_scope(context->scope);
	context->topology = SCENE_init_topology();

	for(uint64_t i = 0; i < size; ++i)
	{
		SCENE_device_t *device = SCENE_get_scope(context->scope, i);

		SCENE_assign_device(device, context->topology);

	}
	//SCENE_print_topology(context->topology);
	return true;
}


//----------------------------------------
//
//				SCENE SYMBOLIZE
//
//----------------------------------------

bool SCENE_symbolize_step_context(SCENE_context_t *context)
{
	SCENE_finalize_links_topology(context->topology);
	SCENE_symbolize_topology(context->topology);
	SCENE_print_topology(context->topology);

	return true;

}


//----------------------------------------
//
//				INIT STAGE
//
//----------------------------------------



//----------------------------------------
//
//				RESOLVE STAGE
//
//----------------------------------------



//----------------------------------------
//
//				BUILD STAGE
//
//----------------------------------------




SCENE_context_t *SCENE_alloc_context(SCENE_scene_t *scene)
{
	assert(scene->filled == true && "scene must be filled to init a context");
	if(scene->filled == false)
	{
		return NULL;
	}
	SCENE_context_t *context = calloc(1, sizeof(SCENE_context_t));
	context->scene = scene;
	context->step = SCENE_CONTEXT_STEP_START;
	return context;
}

bool SCENE_step_context(SCENE_context_t *context, SCENE_ctx_step_t step)
{
	assert(step > 0 && step < sizeof(scene_steps)/sizeof(scene_steps[0]) && "must be a valid step defined to ste content");

	bool passes = scene_steps[step](context);
	context->step = step;

	if(passes == false)
	{
		SCENE_print_context(context);
	}
	assert(passes == true && "must pass all ");
	return passes;
}

void SCENE_init_context(SCENE_context_t *context)
{
	assert(context);
	if(context->step != SCENE_CONTEXT_STEP_START)
	{
		assert(0 && "cannot init context that's not at start step");
	}

	for(SCENE_ctx_step_t step = SCENE_CONTEXT_STEP_ALLOC; step <= SCENE_CONTEXT_STEP_SYMBOLIZE; ++step)
	{
		bool passes = SCENE_step_context(context, step);

	}
}



void SCENE_print_context(SCENE_context_t *context)
{
	printf("context at step: %s\n", scene_steps_names[context->step]);
	if(context->step == 0)
	{
		return;
	}
	printf("context last valid at: %s\n", scene_steps_names[context->step - 1]);
	switch(context->step)
	{
		default:

		case SCENE_CONTEXT_STEP_ASSIGN:

		case SCENE_CONTEXT_STEP_VALIDATE:


		case SCENE_CONTEXT_STEP_ALLOC:
			SCENE_print_scope(context->scope);

		case SCENE_CONTEXT_STEP_START:
			break;



	}

}