#ifndef __WS_SIM_TAG_STRING_HEADER__
#define __WS_SIM_TAG_STRING_HEADER__

#include "TAG_tag.h"
#include <stdint.h>

enum WS_SIM_TAG_STRING_fn_type
{
	WS_SIM_TAG_FN_TYPE(STRING, INIT)
	WS_SIM_TAG_FN_TYPE(STRING, GET)
};


struct WS_SIM_TAG_string
{
	char *string;
};

void TAG_string_free(TAG_ptr_t ptr);
void TAG_string_print(TAG_ptr_t ptr, uint64_t tab);
TAG_ptr_t TAG_string_copy(TAG_ptr_t ptr);

extern TAG_prototype_vtable_t TAG_string_vtable;

static TAG_ptr_t TAG_init_string(char *string);
static char *TAG_get_string(TAG_tag_t *tag);


union WS_SIM_TAG_string_arg
{
	TAG_ptr_t (*init)(char *string);
	char *(*get)(TAG_tag_t *tag);
};

#endif