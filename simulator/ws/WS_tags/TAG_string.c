#include "TAG_string.h"
#include "TAG_tag.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 TAG_ptr_t TAG_init_string(char *string);
 char *TAG_get_string(TAG_tag_t *tag);




TAG_string_arg_t init_string;
TAG_string_arg_t get_string;

TAG_string_arg_t init_string =
{
	.init = TAG_init_string,
};

TAG_string_arg_t get_string =
{
	.get = TAG_get_string,
};


void TAG_string_free(TAG_ptr_t ptr)
{
	TAG_string_t *string = ptr.STRING;

	free(string->string);
	free(string);
}

void TAG_string_print(TAG_ptr_t ptr, uint64_t tab)
{
	printf("%s\n", ptr.STRING->string);
}

TAG_ptr_t TAG_string_copy(TAG_ptr_t ptr)
{
	char *str = ptr.STRING->string;
	assert(str);
	return TAG_init_string(str);
}

TAG_ptr_t TAG_init_string(char *string)
{
	TAG_string_t *ptr = calloc(1, sizeof(TAG_string_t));
	assert(ptr);
	ptr->string = strdup(string);
	assert(ptr->string);
	TAG_ptr_t ret;
	ret.STRING = ptr;
	return ret;
}

char *TAG_get_string(TAG_tag_t *tag)
{
	return tag->ptr.STRING->string;
}


TAG_prototype_vtable_t TAG_string_vtable =
{
	.free = TAG_string_free,
	.print = TAG_string_print,
	.copy = TAG_string_copy,
	.size = 2,
	.fn =
	{
		[TAG_FN_STRING_INIT].STRING = &init_string,
		[TAG_FN_STRING_GET].STRING = &get_string,
	}
};

