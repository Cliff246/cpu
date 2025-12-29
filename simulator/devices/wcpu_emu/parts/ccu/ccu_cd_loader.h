#ifndef __WCPU_PART_CCU_CODE_DESCRIPTOR_LOADER_HEADER__
#define __WCPU_PART_CCU_CODE_DESCRIPTOR_LOADER_HEADER__



#include "wcpu_part_signal.h"
#include "wcpu_isa.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct wcpu_ccu_code_description
{
	uint64_t buffer;
	uint64_t address;
}ccu_code_description_t;

//should be temp

#endif