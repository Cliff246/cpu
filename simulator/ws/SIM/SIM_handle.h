#ifndef __SIM_HANDLE_HEADER__
#define __SIM_HANDLE_HEADER__

#include "module_description.h"
#include "object_instance.h"
#include "hashmap.h"

#include "HND_ptr.h"


typedef struct WS_SIM_handle
{
	OBJ_instance_t instance;
	OBJ_vtable_t vtable;
	p_hashtable_t tags;
	HND_ptr_t ptr;

}SIM_handle_t;

#endif