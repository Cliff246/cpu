#ifndef __FABRIC_HEADER__
#define __FABRIC_HEADER__

#include <stdint.h>

#include <stdbool.h>

typedef struct renamed_register
{
	int64_t bin;

	

	char is_float;	
	char id;
	int epoch;
	char fabric;
	
}renamed_reg_t;

#define FABRIC_REGISTERS 32


//TODO Do this well 
typedef struct fabric
{
	renamed_reg_t registers[FABRIC_REGISTERS];

	

	
}fabric_t;

#endif
