#ifndef __WCPU_FABRIC_REGFILE_HEADER__
#define __WCPU_FABRIC_REGFILE_HEADER__

#include <stdint.h>

#define FABRIC_REGISTERS_COUNT 32


typedef struct fabric_reference_register
{
	int16_t reg_epoch;
	char reg_index;
	char reg_branch;
}frreg_t;

typedef struct fabric_register
{
	int64_t value;
	frreg_t ref;
}fabric_register_t;



typedef struct fabric_register_file
{

	fabric_register_t fregs[FABRIC_REGISTERS_COUNT];


}fabric_regfile_t;





#endif