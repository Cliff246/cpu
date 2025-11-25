#ifndef __FABRIC_HEADER__
#define __FABRIC_HEADER__

#include "fabric_part.h"



#define FABRIC_PART_ORDER_ENUM(FCOMP) \
	FCOMP(FAB_MICRO_ALU)		 	  \
	FCOMP(FAB_REGFILE)				  \
	FCOMP(FAB_BUS)					  \
	FCOMP(FAB_CONTROLER)			  \



typedef enum fabric_debug_flag
{
	FABRIC_DEBUG_FLAG_INVAL,
	FABRIC_DEBUG_FLAG_NONE,
	FABRIC_DEBUG_FLAG_LIMITED,
	FABRIC_DEBUG_FLAG_STRONG,
}fabric_debug_flag_t;



typedef struct wcpu_fabric
{

	int fabric_id;
	fabric_debug_flag_t debug_flag;
	fabric_part_t parts[FABRIC_PARTS_LENGTH];

}fabric_t;

#undef X

fabric_t *create_fabric(void);

void step_fabric(fabric_t *fabric);




#endif