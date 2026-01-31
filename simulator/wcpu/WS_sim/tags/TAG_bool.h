#ifndef __WS_SIM_TAG_BOOL_HEADER__
#define __WS_SIM_TAG_BOOL_HEADER__

#include "TAG_prototype.h"
#include <stdbool.h>

struct WS_SIM_TAG_bool
{
	bool boolean;
};
void TAG_bool_free(TAG_ptr_t ptr);

extern TAG_prototype_vtable_t TAG_bool_vtable;

#endif