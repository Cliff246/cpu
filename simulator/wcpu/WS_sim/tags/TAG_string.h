#ifndef __WS_SIM_TAG_STRING_HEADER__
#define __WS_SIM_TAG_STRING_HEADER__

#include "TAG_prototype.h"


struct WS_SIM_TAG_string
{
	char *string;
};

void TAG_string_free(TAG_ptr_t ptr);

extern TAG_prototype_vtable_t TAG_string_vtable;


#endif