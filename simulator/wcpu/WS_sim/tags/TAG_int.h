#ifndef __WS_SIM_TAG_INT_HEADER__
#define __WS_SIM_TAG_INT_HEADER__

#include "TAG_prototype.h"
#include <stdint.h>

struct WS_SIM_TAG_int
{
	int64_t integer;
};

void TAG_int_free(TAG_ptr_t ptr);

extern TAG_prototype_vtable_t TAG_int_vtable;


#endif