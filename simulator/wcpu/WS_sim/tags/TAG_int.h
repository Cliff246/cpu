#ifndef __WS_SIM_TAG_INT_HEADER__
#define __WS_SIM_TAG_INT_HEADER__

#include "TAG_tag.h"
#include <stdint.h>



enum WS_SIM_TAG_int_fn_type
{
	WS_SIM_TAG_FN_TYPE(INT, INIT)
};

struct WS_SIM_TAG_int
{
	int64_t integer;
};

void TAG_int_free(TAG_ptr_t ptr);
void TAG_int_print(TAG_ptr_t ptr);

static TAG_ptr_t TAG_init_int(int64_t integer);


extern TAG_prototype_vtable_t TAG_int_vtable;

union WS_SIM_TAG_int_arg
{
	TAG_ptr_t (*init)(int64_t integer);
};


#endif