#ifndef __WS_SIMULATOR_BUS_ENTRY_HEADER__
#define __WS_SIMULATOR_BUS_ENTRY_HEADER__

#include <stdint.h>
#include <stdlib.h>

typedef struct wcpu_simulator_bus_slot
{

	uint64_t start, len;
	size_t dev_index;



}WS_simulator_bus_slot_t;

void WS_simulator_print_bus_slot(WS_simulator_bus_slot_t *entry, int index);
WS_simulator_bus_slot_t *WS_simulator_create_bus_slot(uint64_t base, uint64_t len, size_t dev_index);


#endif