#ifndef __WS_SIM_ROUTE_HEADER__
#define __WS_SIM_ROUTE_HEADER__


#include "OBJ_constants.h"

#include "SIM_commons.h"

#include <stdint.h>
#include <stdalign.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct WS_SIM_route
{

	uint64_t address;
	uint64_t length;
	int8_t count;
	int8_t bridges[OBJ_MAX_CHANNELS];
}SIM_route_t;

typedef struct WS_SIM_routemap
{
	//number of
	uint8_t rows;
	//for all channels
	SIM_route_t map[OBJ_MAX_CHANNELS];
	//this is a flat array of every channel that can address something on a wire
}SIM_routemap_t;

//TODO generate an in order routetable for the object
void SIM_routemap_init(SIM_routemap_t *map);
int32_t SIM_routemap_search(SIM_routemap_t *map, uint64_t address);
bool SIM_routemap_has_overlap(SIM_routemap_t *map, uint8_t index);


#endif