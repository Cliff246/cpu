#include "TAG_string.h"
#include "TAG_tag.h"
#include <assert.h>
#include <stdlib.h>

void TAG_string_free(TAG_ptr_t ptr)
{
	TAG_string_t *string = ptr.STRING;

	free(string->string);
	free(string);
}




void TAG_string_free(TAG_ptr_t ptr);


static TAG_ptr_t TAG_init_string(char *string)
{
	assert(0);
}

TAG_string_arg_t init_string =
{
	.init = TAG_init_string,
};

TAG_prototype_vtable_t TAG_string_vtable =
{
	.free = TAG_string_free,
	.size = 1,
	.fn =
	{
		[TAG_FN_STRING_INIT].STRING = &init_string,
	}
};

