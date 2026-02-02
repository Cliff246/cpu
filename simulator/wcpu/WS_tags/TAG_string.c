#include "TAG_string.h"
#include "TAG_tag.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void TAG_string_free(TAG_ptr_t ptr)
{
	TAG_string_t *string = ptr.STRING;
	free(string->string);
	free(string);
}


void TAG_string_print(TAG_ptr_t ptr)
{
	printf("%s\n", ptr.STRING->string);
}



static TAG_ptr_t TAG_init_string(char *string)
{
	TAG_string_t *ptr = calloc(1, sizeof(TAG_string_t));
	assert(ptr);
	ptr->string = strdup(string);
	assert(ptr->string);
	TAG_ptr_t ret;
	ret.STRING = ptr;
	return ret;
}

static char *TAG_get_string(TAG_tag_t *tag)
{
	return tag->ptr.STRING->string;
}

TAG_string_arg_t init_string =
{
	.init = TAG_init_string,
};

TAG_string_arg_t get_string =
{
	.get = TAG_get_string,
};

TAG_prototype_vtable_t TAG_string_vtable =
{
	.free = TAG_string_free,
	.print = TAG_string_print,
	.size = 2,
	.fn =
	{
		[TAG_FN_STRING_INIT].STRING = &init_string,
		[TAG_FN_STRING_GET].STRING = &get_string,
	}
};

