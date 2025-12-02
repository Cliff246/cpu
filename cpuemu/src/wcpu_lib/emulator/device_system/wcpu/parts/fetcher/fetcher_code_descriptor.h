#ifndef __WCPU_PART_FETCHER_CODE_DESCRIPTOR_HEADER__
#define __WCPU_PART_FETCHER_CODE_DESCRIPTOR_HEADER__

#include <stdint.h>

typedef struct wcpu_fetcher_code_descriptor
{
	int64_t element[6];
	uint64_t address;
	
}fetcher_code_descriptor_t;


#endif