#ifndef __WCPU_FABRIC_SHARED_INTERNALS_HEADER__
#define __WCPU_FABRIC_SHARED_INTERNALS_HEADER__

#include <stdbool.h>
#include <stdint.h>


typedef struct wcpu_fabric_reference_register
{
	int16_t epoch;
	int16_t branch;
	int16_t index;

}fabric_regref_t;

#endif