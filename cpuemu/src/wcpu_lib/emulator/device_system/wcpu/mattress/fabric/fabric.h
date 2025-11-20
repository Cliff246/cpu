#ifndef __FABRIC_HEADER__
#define __FABRIC_HEADER__

#include "fabric_lane.h"
#include "alu.h"
#include "fabric_register.h"
#include "fabric_controler.h"
#include "fabric_bus.h"

#define COMPONENT_TYPES_LIST(X)	\
	X(ALU)						\
	X(REGISTERS)				\
	X(CONTROLER)				\
	X(BUS)						\

#define COMPONENT_NAME(C) FABRIC_COMPONENT_ ## C

#define X(x) COMPONENT_NAME(x),
typedef enum fabric_component_type
{
	COMPONENT_TYPES_LIST(X)
}fabric_component_type_t;

#undef X

#undef X
#define X(x) + 1

#define COMPONENTS_LENGTH (0 COMPONENT_TYPES_LIST(X))

typedef struct fabric_component
{
	fabric_component_type_t type;

	union
	{
		alu_t *alu;
		fabric_register_file_t *fabregs;
		fabric_control_t *controler;
		fabric_bus_t *bus;
	}component;

}fabric_component_t;


#define FABRIC_COMPONENT_ORDER_ENUM(FCOMP) \
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



typedef struct fabric
{

	int fabric_id;
	fabric_debug_flag_t debug_flag;
	fabric_component_t components[COMPONENTS_LENGTH];




}fabric_t;

fabric_t *create_fabric(void);

void step_fabric(fabric_t *fabric);




#endif