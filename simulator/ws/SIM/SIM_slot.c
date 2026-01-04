/*

#include "SIM_slot.h"
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

void WS_simulator_print_bus_slot(WS_simulator_bus_slot_t *entry, int index)
{
	printf("slot:%d, start:%d, len:%d, dev_index:%d\n", index, entry->start, entry->len, entry->dev_index);
}

WS_simulator_bus_slot_t *WS_simulator_create_bus_slot(uint64_t base, uint64_t len, size_t dev_index)
{
	WS_simulator_bus_slot_t *entry =  calloc(1, sizeof(WS_simulator_bus_slot_t));
	assert(entry);
	entry->start = base;
	entry->len = len;
	entry->dev_index = dev_index;
	return entry;
}

*/