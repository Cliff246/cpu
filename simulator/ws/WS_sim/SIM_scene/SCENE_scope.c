#include "SCENE_scope.h"
#include "SCENE_anchor.h"
#include "SCENE_device.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

SCENE_scope_t *SCENE_init_scope( uint64_t count)
{
	SCENE_scope_t *scope = calloc(1, sizeof(SCENE_scope_t));
	assert(scope);

	scope->devices = calloc(count, sizeof(SCENE_device_t *));
	scope->devices_count = count;
	scope->filled = false;
	return scope;
}



//add device ptr to scope
void SCENE_fill_scope(SCENE_scope_t *scope, SCENE_device_t **devices, uint64_t count)
{
	assert(scope->devices_count == count);
	assert(!scope->filled && "must not have device scope be filled");
	for(uint64_t i = 0; i < scope->devices_count; ++i)
	{
		scope->devices[i] = devices[i];
	}
	scope->filled = true;
}

//get device from scope
SCENE_device_t *SCENE_get_scope(SCENE_scope_t *scope, uint64_t index)
{
	assert(scope->filled);
	if(index >= scope->devices_count)
	{
		return NULL;
	}
	return scope->devices[index];
}

uint64_t SCENE_get_count_scope(SCENE_scope_t *scope)
{
	assert(scope->filled);
	return scope->devices_count;
}

void SCENE_print_scope(SCENE_scope_t *scope)
{
	if(scope->filled == false)
	{
		return;
	}
	for(uint64_t i = 0; i < scope->devices_count; ++i)
	{
		SCENE_print_device(scope->devices[i]);
	}
}