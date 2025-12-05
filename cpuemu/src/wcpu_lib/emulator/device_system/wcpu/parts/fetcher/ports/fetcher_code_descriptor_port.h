#ifndef __WCPU_PART_FETCHER_CODE_DESCRIPTOR_PORT_HEADER__
#define __WCPU_PART_FETCHER_CODE_DESCRIPTOR_PORT_HEADER__

#include <stdint.h>

#include "fetcher_port_ptr.h"

#define WCPU_CODE_DESCRIPTOR_SIZE 6

typedef struct wcpu_fetcher_port_code_descriptor
{
	int64_t buffer[WCPU_CODE_DESCRIPTOR_SIZE];
	uint32_t index;


}fetcher_code_descriptor_t;


fetcher_port_ptr_t fetcher_port_code_description_create(void);



#endif