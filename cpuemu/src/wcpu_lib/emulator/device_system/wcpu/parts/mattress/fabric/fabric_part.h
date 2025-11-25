#ifndef __WCPU_FABRIC_PART_HEADER__
#define __WCPU_FABRIC_PART_HEADER__

#include "fabric_part_ptr.h"

#include "fabric_lane.h"
#include "fabric_alu.h"
#include "fabric_regfile.h"
#include "fabric_controller.h"
#include "fabric_bus.h"
#include "fabric_executer.h"



//not the same as wcpu_part
typedef struct wcpu_fabric_part
{
	int fabric_part_id;
	fabric_part_type_t type;
	fabric_part_ptr_t pptr;

}fabric_part_t;


#endif