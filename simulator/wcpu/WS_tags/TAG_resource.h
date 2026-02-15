#ifndef __TAG_RESOURCE__
#define __TAG_RESOURCE__

#include "TAG_tag.h"

enum WS_SIM_TAG_RESOURCE_fn_type
{
	WS_SIM_TAG_FN_TYPE(RESOURCE, INIT)
};


struct WS_SIM_TAG_resource
{

};

void TAG_resource_free(TAG_ptr_t ptr);
void TAG_resource_print(TAG_ptr_t ptr);
TAG_ptr_t TAG_resource_copy(TAG_ptr_t ptr);
static TAG_ptr_t TAG_init_resource(void);

extern TAG_prototype_vtable_t TAG_resource_vtable;

union WS_SIM_TAG_resource_arg
{
	TAG_ptr_t (*init)(void);
};

#endif