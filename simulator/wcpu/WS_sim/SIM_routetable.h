#ifndef __WS_SIM_ROUTETABLE_HEADER__
#define __WS_SIM_ROUTETABLE_HEADER__


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
	int32_t overlap_start;
	int32_t overlap_length;
}SIM_route_t;

typedef struct WS_SIM_routetable
{
	//number of
	uint8_t rows;
	uint32_t overlap_size;

	//for all channels
	SIM_route_t table[OBJ_MAX_CHANNELS];
	//this is a flat array of every channel that can address something on a wire
	SIM_channel_local_t overlap[OBJ_MAX_CHANNELS * OBJ_MAX_CHANNELS];
}SIM_routetable_t;

//TODO generate an in order routetable for the object
SIM_routetable_t SIM_routetable_init(void);
int32_t SIM_routetable_search(SIM_routetable_t *rt, uint64_t address);
bool SIM_routetable_overlaps(SIM_routetable_t *rt, uint8_t route);


#endif