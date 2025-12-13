#ifndef __WCPU_FABRIC_PART_REGFILE_HEADER__
#define __WCPU_FABRIC_PART_REGFILE_HEADER__

#include "fabric_internals.h"
#include <stdint.h>




typedef struct wcpu_fabric_register
{
	int64_t value;
	fabric_regref_t rename;
}fabric_register_t;

#define FABRIC_REGISTERS_COUNT 64


typedef struct wcpu_fabric_regfile
{

	fabric_register_t fregs[FABRIC_REGISTERS_COUNT];
	

}fabric_regfile_t;





#endif