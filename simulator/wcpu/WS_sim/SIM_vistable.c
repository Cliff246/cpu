#include "SIM_vistable.h"
#include "SIM_commons.h"
#include "SIM_devcfg.h"
#include "SIM_context.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

SIM_dtag_t SIM_init_viselm(SIM_viselm_t *elm, SIM_devcfg_t *device)
{
	elm->device = device;
	elm->has_address = device->has_address;
	elm->length = device->length;
	elm->address = device->address;
	elm->pretag = device->has_pretag;
	elm->tag = device->pretag;
	return (elm->pretag)? elm->tag : -1;
}

SIM_vistable_t *SIM_init_vistable(SIM_context_t *ctx)
{
	SIM_vistable_t *table = calloc(1, sizeof(SIM_vistable_t));
	assert(table);

	table->table = calloc(ctx->deviceconfigs->count, sizeof(SIM_viselm_t));
	assert(table->table);

	table->tagmap = calloc(ctx->deviceconfigs->count, sizeof(SIM_viselm_t *));
	assert(table->table);

	table->count = ctx->deviceconfigs->count;

	for(uint64_t i = 0; i < table->count; ++i)
	{
		SIM_dtag_t tag = SIM_init_viselm(&table->table[i], &ctx->deviceconfigs->cfgs[i]);
		if(tag >= 0)
		{
			if(table->tagmap[tag % table->count] == NULL)
			{
				table->tagmap[tag % table->count] = &table->table[i];
			}
			else
			{
				//this is overlap case of a map, this shouldnt happen but our method of keying is brittle
				assert(0);
			}

		}
	}


	//TODO make this a good map
	for(uint64_t k = 0; k < table->count; ++k)
	{
		SIM_viselm_t *elm = &table->table[k];
		SIM_viselm_t *cmp = table->tagmap[elm->tag % table->count];
		if(elm == cmp)
		{

		}
		else
		{

		}
	}


	return table;
}


