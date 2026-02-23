#include "CFG_map.h"
#include "CFG_commons.h"
#include "CFG_node.h"
#include "CFG_context.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

CFG_node_id_t CFG_init_mapelm(CFG_mapelm_t *elm, CFG_node_t *node)
{
	elm->device = node;
	elm->has_address = node->has_address;
	elm->length = node->length;
	elm->address = node->address;
	elm->pretag = node->has_pretag;
	elm->tag = node->pretag;
	return (elm->pretag)? elm->tag : -1;
}
bool CFG_init_mapkey(CFG_mapkey_t *key, CFG_mapelm_t *elm)
{
	if(elm->has_address == true)
	{
		key->elm = elm;
		key->size = elm->length;
		key->base = elm->address;
		return true;
	}
	else
	{
		return false;

	}
}
static inline bool CFG_overlap_mapkey(CFG_mapkey_t *key1, CFG_mapkey_t *key2)
{
	return key1->base + key1->size > key2->base;
}
static void CFG_swap_mapkey_list(CFG_mapkey_t *keys, uint64_t i, uint64_t j)
{

	CFG_mapkey_t temp = keys[i];
	keys[i] = keys[j];
	keys[j] = temp;
}

static uint64_t CFG_partition_mapkey_list(CFG_mapkey_t *list, uint64_t lo, uint64_t hi)
{
	uint64_t pivot = list[hi].base;

	uint64_t i = lo;

	for(uint64_t j = lo; j <= (hi - 1); ++j)
	{
		if(list[j].base < pivot)
		{
           	CFG_swap_mapkey_list(list, i, j);
            i++;
		}
	}

    CFG_swap_mapkey_list(list, i, hi);
    return i;
}
static void CFG_qsort_mapkey_list(CFG_mapkey_t *list, uint64_t lo, uint64_t hi)
{
	if(lo >= hi) return;
	uint64_t pivot = CFG_partition_mapkey_list(list, lo, hi);
	if(pivot > 0)
		CFG_qsort_mapkey_list(list, lo, pivot - 1);
	CFG_qsort_mapkey_list(list, pivot + 1, hi);
}

static bool CFG_check_no_collision_mapkey_list(CFG_mapkey_t *list, uint64_t size)
{
	bool passed = true;
	for(uint64_t i = 0; i < size - 1; ++i)
	{
		CFG_mapkey_t *last = &list[i];
		CFG_mapkey_t *next = &list[i + 1];
		passed &= CFG_overlap_mapkey(last, next);

	}
	return passed;
}
static CFG_mapkey_t *CFG_bsearch_mapkey_list(CFG_mapkey_t *list, uint64_t size, uint64_t i)
{
	uint64_t lo = 0;
	uint64_t hi = size;
	while(lo < hi)
	{
		uint64_t mi = lo + (hi - lo) / 2;
		CFG_mapkey_t *elm = &list[mi];
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

CFG_map_t *CFG_init_map(CFG_context_t *ctx)
{
	CFG_map_t *table = calloc(1, sizeof(CFG_map_t));
	assert(table);

	table->table = calloc(ctx->deviceconfigs->count, sizeof(CFG_mapelm_t));
	assert(table->table);

	table->tagmap = calloc(ctx->deviceconfigs->count, sizeof(CFG_mapelm_t *));
	assert(table->tagmap);

	table->count = ctx->deviceconfigs->count;

	table->addrmap = calloc(ctx->deviceconfigs->count, sizeof(CFG_mapkey_t));
	assert(table->addrmap);
	table->addrmap_count = 0;


	CFG_init_tags_map(table, ctx);
	CFG_init_tagmap_map(table, ctx);
	CFG_init_addrmap_map(table, ctx);
	//TODO make this a good map
 	bool aligned = CFG_init_addrmap_align_map(table, ctx);
	if(aligned == true)
	{
		assert(0 && "addresses overlaped");

		exit(EXIT_FAILURE);
	}

	//CFG_print_table_map(table);
	return table;
}
static void CFG_init_tags_map(CFG_map_t *table, CFG_context_t *ctx)
{
	for(uint64_t i = 0; i < table->count; ++i)
	{
		CFG_node_id_t tag = CFG_init_mapelm(&table->table[i], &ctx->deviceconfigs->cfgs[i]);

		bool success = CFG_add_from_key_map(table, &table->table[i], tag);

	}
}
static void CFG_init_tagmap_map(CFG_map_t  *table, CFG_context_t *ctx)
{
	for(uint64_t k = 0; k < table->count; ++k)
	{
		CFG_mapelm_t *elm = &table->table[k];
		if(elm->pretag == false)
		{
			int32_t count = 0;
			do
			{
				elm->tag = CFG_generate_tag(elm->device);
				bool success = CFG_add_from_key_map(table, elm, elm->tag);
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

			assert(CFG_get_from_key_map(table, elm->tag) == elm);
			elm->pretag = true;
		}
	}
}

static void CFG_init_addrmap_map(CFG_map_t  *table, CFG_context_t *ctx)
{
	for(uint64_t j = 0; j < table->count; ++j)
	{
		CFG_mapelm_t *elm = &table->table[j];
		CFG_mapkey_t *addr = &table->addrmap[table->addrmap_count];
		bool has_address = CFG_init_mapkey(addr, elm);
		if(has_address)
			table->addrmap_count++;

	}
}
static bool CFG_init_addrmap_align_map(CFG_map_t  *table, CFG_context_t *ctx)
{
	CFG_qsort_mapkey_list(table->addrmap, 0, table->addrmap_count - 1);
	bool test = CFG_check_no_collision_mapkey_list(table->addrmap, table->addrmap_count);

	return test;
}
bool CFG_add_from_key_map(CFG_map_t *table, CFG_mapelm_t *elm, CFG_node_id_t tag)
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
CFG_mapelm_t *CFG_get_from_key_map(CFG_map_t *table, CFG_node_id_t tag)
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

				CFG_mapelm_t *ptr = table->tagmap[cur];
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
CFG_mapelm_t *CFG_get_from_addr_map(CFG_map_t *table, uint64_t addr)
{
	CFG_mapkey_t *addrptr = CFG_bsearch_mapkey_list(table->addrmap, table->addrmap_count, addr);
	if(addrptr == NULL)
	{
		return NULL;
	}
	CFG_mapelm_t *elm = addrptr->elm;

	return elm;
}
CFG_mapelm_t *CFG_get_from_index_map(CFG_map_t *table, uint64_t i)
{
	assert(i < table->count);
	CFG_mapelm_t *elm = &table->table[i];
	return elm;
}

void CFG_print_mapelm(CFG_mapelm_t *elm)
{
	printf("viselm ptr:%p, address: %d[%ld %ld], tag:%d[%ld]\n", elm->device, elm->has_address, elm->address, elm->length, elm->pretag, elm->tag);
}

//this is very raw
void CFG_free_map(CFG_map_t *table)
{
	free(table->tagmap);
	free(table->table);
	free(table->addrmap);
	free(table);

}
void CFG_print_table_map(CFG_map_t *table)
{
	for(uint64_t i = 0; i < table->count; ++i)
	{
		CFG_print_mapelm(&table->table[i]);
	}

}
void CFG_print_tagmap_map(CFG_map_t *table)
{
	for(uint64_t i = 0; i < table->count; ++i)
	{
		printf("[%ld]=%p\n",i, table->tagmap[i]);

	}
}
void CFG_print_addrmap_map(CFG_map_t *table)
{
	for(uint64_t i = 0; i < table->addrmap_count; ++i)
	{
		printf("[%ld]=(%ld, %ld)\n", i, table->addrmap[i].base, table->addrmap[i].size);
	}
}