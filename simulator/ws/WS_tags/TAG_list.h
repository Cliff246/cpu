#ifndef __WS_SIM_TAG_LIST_HEADER__
#define __WS_SIM_TAG_LIST_HEADER__

#include "TAG_tag.h"
#include <stdint.h>
#include <stdbool.h>



enum WS_SIM_TAG_LIST_fn_type
{
	WS_SIM_TAG_FN_TYPE(LIST, INIT_EMPTY)
	WS_SIM_TAG_FN_TYPE(LIST, INIT_BOOLS)
	WS_SIM_TAG_FN_TYPE(LIST, INIT_STRINGS)
	WS_SIM_TAG_FN_TYPE(LIST, INIT_INTS)
	WS_SIM_TAG_FN_TYPE(LIST, INIT_TAGS)
	WS_SIM_TAG_FN_TYPE(LIST, GET)
	WS_SIM_TAG_FN_TYPE(LIST, GET_SIZE)
	WS_SIM_TAG_FN_TYPE(LIST, APPEND)
};


struct WS_SIM_TAG_list
{
	uint64_t size;
	uint64_t alloc;
	TAG_tag_t **list;
};

void TAG_list_free(TAG_ptr_t ptr);
void TAG_list_print(TAG_ptr_t ptr, uint64_t tab);
TAG_ptr_t TAG_list_copy(TAG_ptr_t ptr);

extern TAG_prototype_vtable_t TAG_list_vtable;

union WS_SIM_TAG_list_arg
{
	TAG_ptr_t (*init_empty)(void);
	TAG_ptr_t (*init_bools)(uint64_t size, bool *bools);
	TAG_ptr_t (*init_strings)(uint64_t size, char **strings);
	TAG_ptr_t (*init_ints)(uint64_t size, int64_t *integer);
	TAG_ptr_t (*init_tags)(uint64_t size, TAG_tag_t **tags);
	TAG_tag_t *(*get)(TAG_tag_t *tag, uint64_t pos);
	uint64_t (*get_size)(TAG_tag_t *tag);
	void (*append)(TAG_tag_t *tag, TAG_tag_t *add);
};


#endif