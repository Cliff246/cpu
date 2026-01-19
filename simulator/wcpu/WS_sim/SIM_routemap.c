#include "SIM_routemap.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

void SIM_routemap_init(SIM_routemap_t *map)
{
	assert(0 && "TODO sim routetable init");
}

int32_t SIM_routemap_search(SIM_routemap_t *map, uint64_t address)
{
	assert(map);
	assert(map->rows > 0);
	uint64_t low = 0;
	uint64_t high = map->rows - 1;
	uint64_t mid = 0;
	while(low <= high)
	{
		mid = low + (high - low) / 2;
		uint64_t start = map->map[mid].address;
		uint64_t end = map->map[mid].length + start;
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
bool SIM_routemap_has_overlap(SIM_routemap_t *map, uint8_t index)
{
	assert(index < map->rows);

	SIM_route_t *route = &map->map[index];
	if(route->count > 0)
	{
		return true;

	}
	return false;
}