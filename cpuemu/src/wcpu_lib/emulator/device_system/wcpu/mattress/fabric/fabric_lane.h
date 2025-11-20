#ifndef __LANE_RESOLVER_HEADER__
#define __LANE_RESOLVER_HEADER__

#include <stdint.h>
#include <stdbool.h>

#include "fabric_controler.h"

typedef struct fabric_lane
{
	int64_t data;

	int16_t reg_tag;
	int16_t reg_epoch;
	int16_t reg_branch;
}fabric_lane_t;




typedef struct fabric_lanes
{
	bool ready;
	char dest;
	fabric_lane_t lane1;
	fabric_lane_t lane2;
	fabric_lane_t lane3;
	int32_t opcode;



}fabric_lanes_t;




fabric_lanes_t *create_fabric_lane(void);





#endif