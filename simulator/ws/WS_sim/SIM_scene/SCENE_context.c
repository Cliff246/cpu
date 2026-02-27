#include "SCENE_context.h"


/*

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

*/