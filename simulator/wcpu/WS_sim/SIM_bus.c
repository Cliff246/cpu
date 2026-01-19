#include "SIM_bus.h"
#include "SIM_commons.h"
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>


SIM_bus_address_element_t *SIM_bus_address_search(SIM_bus_t *bus, uint64_t address)
{
	assert(bus && "bus not be empty");
	assert(bus->finished);
	if(bus->address_table.count == 0)
		return NULL;
	assert(bus->address_table.addresses && "must be instanced");
	uint64_t low = 0;
	uint64_t high = bus->address_table.count - 1;
	uint64_t mid = 0;
	while(low <= high)
	{
		mid = low + (high - low) / 2;
		uint64_t start = bus->address_table.addresses[mid].address;
		uint64_t end = bus->address_table.addresses[mid].length + start;
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
			return &bus->address_table.addresses[mid];

		}
	}
	return NULL;
}

SIM_tag_t SIM_bus_find_address(SIM_bus_t *bus, uint64_t address)
{
	assert(bus);
	assert(bus->finished == true);

 	SIM_bus_address_element_t *elem = SIM_bus_address_search(bus, address);
	if(elem == NULL)
		return -1;
	else
	{
		return elem->tag;
	}
}

int16_t SIM_bus_find_local_channel(SIM_bus_t *bus, SIM_tag_t tag)
{
	assert(bus);
	assert(bus->finished == true);
	if(tag > SIM_TAGS_SIZE)
		return -1;
	else
	{
		return bus->tag_table.tags[tag];
	}
}