#ifndef __TAG_MAP_HEADER__
#define __TAG_MAP_HEADER__

#include "TAG_tag.h"

enum WS_SIM_TAG_MAP_fn_type
{
	WS_SIM_TAG_FN_TYPE(MAP, INIT)

};

struct WS_SIM_TAG_map
{

};

void TAG_map_free(TAG_ptr_t ptr);
void TAG_map_print(TAG_ptr_t ptr);
TAG_ptr_t TAG_map_copy(TAG_ptr_t ptr);
static TAG_ptr_t TAG_init_map(void);


extern TAG_prototype_vtable_t TAG_map_vtable;

union WS_SIM_TAG_map_arg
{
	TAG_ptr_t (*init)(void);
};

#endif