#ifndef __MATTRESS_HEADER__
#define __MATTRESS_HEADER__

#define MATTRESS_TOTAL_FABRICS 4

#include "fabric.h"

typedef struct mattress
{
	fabric_t fabrics[MATTRESS_TOTAL_FABRICS];
}mattress_t;

#endif