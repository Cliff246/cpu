#ifndef __RUN_OBJECT_HEADER__
#define __RUN_OBJECT_HEADER__


#include "OBJ_vtable.h"

typedef struct RUN_object
{
	OBJ_vtable_t vtable;
}RUN_obj_t;

#endif