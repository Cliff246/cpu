#include "SIM_vistable.h"
#include "SIM_commons.h"
#include "SIM_devcfg.h"
#include "SIM_context.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

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

bool SIM_init_visaddr(SIM_visaddr_t *address, SIM_viselm_t *elm)
{
	if(elm->has_address == true)
	{
		address->elm = elm;
		address->size = elm->length;
		address->base = elm->address;
		return true;
	}
	else
	{
		return false;

	}
}

static inline bool SIM_overlap_visaddr(SIM_visaddr_t *addr1, SIM_visaddr_t *addr2)
{
	return addr1->base + addr1->size > addr2->base;
}

static void SIM_swap_visaddr_list(SIM_visaddr_t *list, uint64_t i, uint64_t j)
{

	SIM_visaddr_t temp = list[i];
	list[i] = list[j];
	list[j] = temp;
}


static uint64_t SIM_partition_visaddr_list(SIM_visaddr_t *list, uint64_t lo, uint64_t hi)
{
	uint64_t pivot = list[hi].base;

	uint64_t i = lo;

	for(uint64_t j = lo; j <= (hi - 1); ++j)
	{
		if(list[j].base < pivot)
		{
           	SIM_swap_visaddr_list(list, i, j);
            i++;
		}
	}

    SIM_swap_visaddr_list(list, i, hi);
    return i;
}

static void SIM_qsort_visaddr_list(SIM_visaddr_t *list, uint64_t lo, uint64_t hi)
{
	if(lo >= hi) return;
	uint64_t pivot = SIM_partition_visaddr_list(list, lo, hi);
	if(pivot > 0)
		SIM_qsort_visaddr_list(list, lo, pivot - 1);
	SIM_qsort_visaddr_list(list, pivot + 1, hi);
}

static bool SIM_check_no_collisions_visaddr_list(SIM_visaddr_t *list, uint64_t size)
{
	bool passed = true;
	for(uint64_t i = 0; i < size - 1; ++i)
	{
		SIM_visaddr_t *last = &list[i];
		SIM_visaddr_t *next = &list[i + 1];
		passed &= SIM_overlap_visaddr(last, next);

	}
	return passed;
}

static SIM_visaddr_t *SIM_bsearch_visaddr_list(SIM_visaddr_t *list, uint64_t size, uint64_t i)
{
	uint64_t lo = 0;
	uint64_t hi = size;
	while(lo < hi)
	{
		uint64_t mi = lo + (hi - lo) / 2;
		SIM_visaddr_t *elm = &list[mi];
		if(i < elm->base )
		{
			hi = mi;
		}
		else if(i - elm->base >= elm->size)
		{
			lo = mi + 1;
		}
		else
			return &list[mi];

	}
	return NULL;
}


SIM_vistable_t *SIM_init_vistable(SIM_context_t *ctx)
{
	SIM_vistable_t *table = calloc(1, sizeof(SIM_vistable_t));
	assert(table);

	table->table = calloc(ctx->deviceconfigs->count, sizeof(SIM_viselm_t));
	assert(table->table);

	table->tagmap = calloc(ctx->deviceconfigs->count, sizeof(SIM_viselm_t *));
	assert(table->tagmap);

	table->count = ctx->deviceconfigs->count;

	table->addrmap = calloc(ctx->deviceconfigs->count, sizeof(SIM_visaddr_t));
	assert(table->addrmap);
	table->addrmap_count = 0;


	SIM_init_vistable_tags(table, ctx);
	SIM_init_vistable_tagmap(table, ctx);
	SIM_init_vistable_addrmap(table, ctx);
	//TODO make this a good map
 	bool aligned = SIM_init_vistable_addrmap_align(table, ctx);
	if(aligned == true)
	{
		assert(0 && "addresses overlaped");

		exit(EXIT_FAILURE);
	}

	//SIM_viselm_t *elm =  SIM_get_from_addr_vistable(table, 100);
	//SIM_print_viselm(elm);

	SIM_print_vistable_table(table);
	//SIM_print_vistable_addrmap(table);
	return table;
}

static void SIM_init_vistable_tags(SIM_vistable_t *table, SIM_context_t *ctx)
{
	for(uint64_t i = 0; i < table->count; ++i)
	{
		SIM_dtag_t tag = SIM_init_viselm(&table->table[i], &ctx->deviceconfigs->cfgs[i]);

		bool success = SIM_add_from_key_vistable(table, &table->table[i], tag);

	}
}

static void SIM_init_vistable_tagmap(SIM_vistable_t *table, SIM_context_t *ctx)
{
	for(uint64_t k = 0; k < table->count; ++k)
	{
		SIM_viselm_t *elm = &table->table[k];
		if(elm->pretag == false)
		{
			int32_t count = 0;
			do
			{
				elm->tag = elm->tag = ((uint64_t)rand() << 32) + rand();
				bool success = SIM_add_from_key_vistable(table, elm, elm->tag);
				if(success == true)
				{
					break;
				}
				count++;
			}while(count < 10);

			if(count >= 10)
			{
				assert(0);
				exit(EXIT_FAILURE);
			}

			assert(SIM_get_from_key_vistable(table, elm->tag) == elm);
			elm->pretag = true;
		}
	}
}

static void SIM_init_vistable_addrmap(SIM_vistable_t *table, SIM_context_t *ctx)
{
	for(uint64_t j = 0; j < table->count; ++j)
	{
		SIM_viselm_t *elm = &table->table[j];
		SIM_visaddr_t *addr = &table->addrmap[table->addrmap_count];
		bool has_address = SIM_init_visaddr(addr, elm);
		if(has_address)
			table->addrmap_count++;

	}
}

static bool SIM_init_vistable_addrmap_align(SIM_vistable_t *table, SIM_context_t *ctx)
{
	SIM_qsort_visaddr_list(table->addrmap, 0, table->addrmap_count - 1);
	bool test = SIM_check_no_collisions_visaddr_list(table->addrmap, table->addrmap_count);

	return test;
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
		for(uint32_t i = 0, cur = pos; i < table->count; ++i, cur = (cur + 1) % table->count)
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
		else if (table->tagmap[pos]->tag == tag)
		{
			return table->tagmap[pos];
		}
		else
		{
			for(uint32_t i = 0, cur = pos; i < table->count; ++i, cur = (cur + 1) % table->count)
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

SIM_viselm_t *SIM_get_from_addr_vistable(SIM_vistable_t *table, uint64_t addr)
{
	SIM_visaddr_t *addrptr = SIM_bsearch_visaddr_list(table->addrmap, table->addrmap_count, addr);
	if(addrptr == NULL)
	{
		return NULL;
	}
	SIM_viselm_t *elm = addrptr->elm;

	return elm;
}

SIM_viselm_t *SIM_get_from_index_vistable(SIM_vistable_t *table, uint64_t i)
{
	assert(i < table->count);
	SIM_viselm_t *elm = &table->table[i];
	return elm;
}

void SIM_print_viselm(SIM_viselm_t *elm)
{
	printf("viselm ptr:%p, address: %d[%ld %ld], tag:%d[%ld]\n", elm->device, elm->has_address, elm->address, elm->length, elm->pretag, elm->tag);
}

//this is very raw
void SIM_free_vistable(SIM_vistable_t *table)
{
	free(table->tagmap);
	free(table->table);
	free(table->addrmap);
	free(table);

}

void SIM_print_vistable_table(SIM_vistable_t *table)
{
	for(uint64_t i = 0; i < table->count; ++i)
	{
		SIM_print_viselm(&table->table[i]);
	}

}

void SIM_print_vistable_tagmap(SIM_vistable_t *table)
{
	for(uint64_t i = 0; i < table->count; ++i)
	{
		printf("[%ld]=%p\n",i, table->tagmap[i]);

	}
}

void SIM_print_vistable_addrmap(SIM_vistable_t *table)
{
	for(uint64_t i = 0; i < table->addrmap_count; ++i)
	{
		printf("[%ld]=(%ld, %ld)\n", i, table->addrmap[i].base, table->addrmap[i].size);
	}
}