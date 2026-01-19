#ifndef __WS_SIM_BUS_HEADER__
#define __WS_SIM_BUS_HEADER__

#include "OBJ_constants.h"
#include "SIM_commons.h"
#include <stdint.h>

typedef struct WS_SIM_bus_address_elemenet
{
	uint64_t address;
	uint64_t length;

	SIM_tag_t tag;
}SIM_bus_address_element_t;

typedef struct WS_SIM_bus_address_table
{
	uint64_t count;
	SIM_bus_address_element_t *addresses;
}SIM_bus_address_table_t;

typedef struct WS_SIM_bus_tag_table
{
	//locale address of the wire
	int16_t tags[SIM_TAGS_SIZE];
}SIM_bus_tag_table_t;

typedef struct WS_SIM_bus
{
	_Alignas(1024) uint64_t wire_id;
	SIM_bus_tag_table_t tag_table;
	SIM_bus_address_table_t address_table;
	bool finished;
}SIM_bus_t;


typedef struct WS_SIM_graph SIM_graph_t;


void SIM_bus_init(SIM_bus_t *bus, uint64_t wire_id);

SIM_bus_address_element_t *SIM_bus_address_search(SIM_bus_t *bus, uint64_t address);
SIM_tag_t SIM_bus_find_address(SIM_bus_t *bus, uint64_t address);
int16_t SIM_bus_find_local_channel(SIM_bus_t *bus, SIM_tag_t tag);

#endif