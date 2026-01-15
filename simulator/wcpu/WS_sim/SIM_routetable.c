#include "SIM_routetable.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

SIM_routetable_t SIM_routetable_init(void)
{
	assert(0 && "TODO sim routetable init");
}

int32_t SIM_routetable_search(SIM_routetable_t *rt, uint64_t address)
{
	assert(rt);
	assert(rt->rows > 0);
	assert(rt->overlap != NULL);
	assert(rt->overlap_size != 0);
	uint64_t low = 0;
	uint64_t high = rt->rows - 1;
	uint64_t mid = 0;
	while(low <= high)
	{
		mid = low + (high - low) / 2;
		uint64_t start = rt->table[mid].address;
		uint64_t end = rt->table[mid].length + start;
		//printf("%d %d\n", start, end);
		if(address < start)
		{
			high = mid - 1;

		}
		else if(address >= end)
		{
			low = mid + 1;
		}
		else
		{
			return mid;

		}
	}
	return -1;
}

bool SIM_routetable_overlaps(SIM_routetable_t *rt, uint8_t route)
{
	assert(route < rt->rows);

	assert(rt->overlap);
	assert(rt->overlap_size > 0);

	SIM_route_t *r = &rt->table[route];

	if(r->overlap_start < 0 || r->overlap_length < 0)
	{
		return false;
	}
	assert(r->overlap_length + r->overlap_start < rt->overlap_size);
	return true;
}