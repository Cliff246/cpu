#ifndef __WS_SIM_TAG_LIST_HEADER__
#define __WS_SIM_TAG_LIST_HEADER__

#include "TAG_tag.h"
#include <stdint.h>
#include <stdbool.h>



enum WS_SIM_TAG_LIST_fn_type
{
	WS_SIM_TAG_FN_TYPE(LIST, INIT_BOOLS)
	WS_SIM_TAG_FN_TYPE(LIST, INIT_STRINGS)
	WS_SIM_TAG_FN_TYPE(LIST, INIT_INTS)

};


struct WS_SIM_TAG_list
{
	uint64_t size;
	TAG_tag_t **list;
};

void TAG_list_free(TAG_ptr_t ptr);
void TAG_list_print(TAG_ptr_t ptr);

static TAG_ptr_t TAG_init_list_bools(uint64_t size, bool *bools);
static TAG_ptr_t TAG_init_list_strings(uint64_t size, char **strings);
static TAG_ptr_t TAG_init_list_ints(uint64_t size, int64_t *integer);





extern TAG_prototype_vtable_t TAG_list_vtable;

union WS_SIM_TAG_list_arg
{
	TAG_ptr_t (*init_bools)(uint64_t size, bool *bools);
	TAG_ptr_t (*init_strings)(uint64_t size, char **strings);
	TAG_ptr_t (*init_ints)(uint64_t size, int64_t *integer);
};


#endif