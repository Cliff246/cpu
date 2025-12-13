#ifndef __WCPU_PART_MATTRESS_UNIT_HEADER__
#define __WCPU_PART_MATTRESS_UNIT_HEADER__

#include "fabric.h"
#include "mattress_interface.h"

typedef struct wcpu_mattress_unit
{
	int id;
	fabric_t fabric;
	mattress_interface_t interface;

}mattress_unit_t;


#endif