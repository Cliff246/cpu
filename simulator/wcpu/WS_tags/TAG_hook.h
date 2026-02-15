#ifndef __TAG_HOOK_HEADER__
#define __TAG_HOOK_HEADER__


#include "TAG_tag.h"

enum WS_SIM_TAG_HOOK_fn_type
{
	WS_SIM_TAG_FN_TYPE(HOOK, INIT)

};

struct WS_SIM_TAG_hook
{

};

void TAG_hook_free(TAG_ptr_t ptr);
void TAG_hook_print(TAG_ptr_t ptr);
TAG_ptr_t TAG_hook_copy(TAG_ptr_t ptr);
static TAG_ptr_t TAG_init_hook(void);


extern TAG_prototype_vtable_t TAG_hook_vtable;

union WS_SIM_TAG_hook_arg
{
	TAG_ptr_t (*init)(void);
};

#endif