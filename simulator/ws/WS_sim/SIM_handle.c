#include "SIM_handle.h"
#include "commons.h"
#include "dynamic_lib.h"
#include "hashmap.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static p_hashtable_t simmodhnds = NULL;




void SIM_free_modhandle(void *ptr)
{
	SIM_modhandle_t *modhnd = ptr;
	free(modhnd->key);
	free(modhnd);
	assert(0);
}

static SIM_modhandle_t *SIM_init_modhandle(char *handle)
{
	SIM_alloc_simmodhnd();
	assert(handle != NULL && "cannot init existing modhandle");

	assert(getdata_from_hash_table(simmodhnds, handle) == NULL);

	SIM_modhandle_t *modhnd = calloc(1, sizeof(SIM_modhandle_t));
	assert(modhnd);


	modhnd->key = strdup(handle);
	assert(modhnd->key);
	modhnd->dl = WS_dynamic_lib_get(modhnd->key);
	assert(modhnd->dl);

	modhnd->desc = modhnd->dl->dev_desc;
	return modhnd;
}

void SIM_alloc_simmodhnd(void)
{
	static bool started = false;
	if(started == false)
	{
		simmodhnds = new_hash_table(SIM_MODHND_MAX, SIM_free_modhandle);
		assert(simmodhnds);
		started = true;
	}
	return;
}

void SIM_modhandle_append(SIM_modhandle_t *modhandle, SIM_handle_t *handle)
{
	assert(modhandle);
	assert(handle);
	if(modhandle->alloc == 0)
	{
		modhandle->alloc = 1;
	}
	modhandle->handles = realloc_safe(modhandle->handles, modhandle->alloc + 1, sizeof(SIM_handle_t *));
	modhandle->handles[modhandle->alloc++] = handle;
}

SIM_handle_t *SIM_init_handle_modhandle(SIM_modhandle_t *modhandle)
{
	SIM_handle_t *handle = calloc(1, sizeof(SIM_handle_t));

	handle->desc = modhandle->desc;
	handle->key = modhandle->key;

	SIM_modhandle_append(modhandle, handle);
	
	return handle;

}

SIM_handle_t *SIM_init_handle(char *handle)
{
	SIM_alloc_simmodhnd();
	SIM_modhandle_t *modhandle = getdata_from_hash_table(simmodhnds, handle);
	if(modhandle == NULL)
	{
		modhandle = SIM_init_modhandle(handle);
		addto_hash_table(simmodhnds, handle, modhandle);

	}

	SIM_handle_t *new = SIM_init_handle_modhandle(modhandle);
	return new;
}


void SIM_free_handle(SIM_handle_t *handle)
{
	assert(0);
}