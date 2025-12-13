#ifndef __WCPU_FABRIC_PART_LANE_HEADER__
#define __WCPU_FABRIC_PART_LANE_HEADER__

#include <stdint.h>
#include <stdbool.h>

#include "fabric_controller.h"
#include "fabric_internals.h"

typedef struct fabric_lane
{
	int64_t data;
	fabric_regref_t ref;
}fabric_lane_t;




typedef struct fabric_lanes
{
	bool ready;
	int dest;
	fabric_lane_t lane1;
	fabric_lane_t lane2;
	fabric_lane_t lane3;
	int32_t opcode;



}fabric_lanes_t;




fabric_lanes_t *create_fabric_lane(void);





#endif