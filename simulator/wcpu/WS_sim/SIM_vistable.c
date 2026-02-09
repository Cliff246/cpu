#include "SIM_vistable.h"
#include "SIM_commons.h"
#include "SIM_devcfg.h"
#include "SIM_context.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
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
		bool success =SIM_add_from_key_vistable(table, &table->table[i], tag);
	}


	//TODO make this a good map
	for(uint64_t k = 0; k < table->count; ++k)
	{
		SIM_viselm_t *elm = &table->table[k];
		if(elm->pretag == false)
		{
			elm->tag = rand();
			SIM_add_from_key_vistable(table, elm, elm->tag);
			assert(SIM_get_from_key_vistable(table, elm->tag) == elm);
			elm->pretag = true;

		}

	}
	SIM_print_vistable(table);

	return table;
}

bool SIM_add_from_key_vistable(SIM_vistable_t *table, SIM_viselm_t *elm, SIM_dtag_t tag)
{

	if(tag < 0)
		return false;


	uint64_t pos = tag % table->count;

	if(table->tagmap[pos] == NULL)
	{
		table->tagmap[pos] = elm;
		return true;
	}
	else
	{
		for(uint32_t i = 0, cur = pos; i < table->count; ++i, cur = (cur >= table->count)? 0: cur + 1)
		{
			if(table->tagmap[cur] == NULL)
			{
				table->tagmap[cur] = elm;
				return true;
			}
			else if(table->tagmap[cur]->tag == tag)
			{
				return false;
			}

		}
		return false;
	}
}

SIM_viselm_t *SIM_get_from_key_vistable(SIM_vistable_t *table, SIM_dtag_t tag)
{
	if(tag >= 0)
	{
		uint64_t pos = tag % table->count;

		if(table->tagmap[pos] == NULL)
		{
			return NULL;
		}
		else if (table->tagmap[pos]->tag != tag)
		{
			return table->tagmap[pos];
		}
		else
		{
			for(uint32_t i = 0, cur = pos; i < table->count; ++i, cur = (cur >= table->count)? 0: cur + 1)
			{

				SIM_viselm_t *ptr = table->tagmap[cur];
				if(ptr == NULL)
				{
					return NULL;
				}
				else if(ptr->tag == tag)
				{
					return ptr;
				}

			}
		}
	}
	return NULL;
}

void SIM_print_vistable(SIM_vistable_t *table)
{
	for(uint64_t i = 0; i < table->count; ++i)
	{
		printf("[%d]=%p\n",i, table->tagmap[i]);
	}
}