#include "RUN_idmap.h"
#include "SIM_commons.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t idmap_index(SIM_dtag_t tag, uint64_t size)
{
	return (tag) % size;
}

void RUN_alloc_idmap(RUN_idmap_t *map, uint64_t size)
{
	map->size = size;
	map->allocd = size * 10;
	map->map = calloc(map->allocd, sizeof(RUN_idval_t));
	for(uint64_t i = 0; i < map->allocd; ++i)
	{
		map->map[i].did = 0;
		map->map[i].dtag = -1;
	}

}

void RUN_build_idmap(RUN_idmap_t *map, SIM_dtag_t *tags, SIM_did_t *ids, uint64_t size)
{

	assert(map->size == size);


	for(uint64_t f = 0; f < size; ++f)
	{
		uint64_t pos = idmap_index(tags[f], map->allocd);
		if(map->map[pos].dtag == -1)
		{
			map->map[pos].dtag = tags[f];
			map->map[pos].did = ids[f];
		}
		else
		{
			bool passed = false;
			for(uint64_t c = 0; c < map->allocd; ++c)
			{
				pos = (pos + 1) % map->allocd;
				if(map->map[pos].dtag == -1)
				{
					map->map[pos].dtag = tags[f];
					map->map[pos].did = ids[f];
					passed = true;
					break;
				}
				//printf("%d %d\n", pos, c);
			}
			assert(passed);
		}
	}



}


SIM_did_t RUN_get_did_idmap(RUN_idmap_t *map, SIM_dtag_t tag)
{

	uint64_t start = idmap_index(tag, map->size);

	for(uint64_t i = 0; i < map->size; ++i)
	{
		if(map->map[start].dtag == tag)
		{
			return map->map[start].did;
		}
		else
		{
			start = (start + 1) % map->size;
		}
	}
	assert(0);
}