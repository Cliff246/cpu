#ifndef __WS_SIM_TAG_BOOL_HEADER__
#define __WS_SIM_TAG_BOOL_HEADER__

#include "TAG_tag.h"
#include <stdbool.h>
#include <stdint.h>

enum WS_SIM_TAG_bool_fn_type
{
	WS_SIM_TAG_FN_TYPE(BOOL, INIT)
	WS_SIM_TAG_FN_TYPE(BOOL, GET)
};

struct WS_SIM_TAG_bool
{
	bool boolean;
};
void TAG_bool_free(TAG_ptr_t ptr);
void TAG_bool_print(TAG_ptr_t ptr, uint64_t tab);
TAG_ptr_t TAG_bool_copy(TAG_ptr_t ptr);

extern TAG_prototype_vtable_t TAG_bool_vtable;



union WS_SIM_TAG_bool_arg
{
	TAG_ptr_t (*init)(bool boolean);
	bool (*get)(TAG_tag_t *tag);
};


#endif