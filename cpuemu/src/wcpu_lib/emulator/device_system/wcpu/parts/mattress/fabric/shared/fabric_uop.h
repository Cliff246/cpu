#ifndef __WCPU_FABRIC_SHARED_UOP__
#define __WCPU_FABRIC_SHARED_UOP__


#include <stdbool.h>
#include <stdint.h>



typedef struct wcpu_fabric_uop
{
	int opcode;

	//the register index's for 
	int regref_rs1, regref_rs2, regref_rs3, regref_rd;

	//the slot in the sidestream that the immediate is in
	int immslot;
	bool accmode;
	bool selflag;
	bool reallocflag;
	bool immflag;

}fabric_uop_t;



#endif