
#include "TAG_bool.h"
#include "TAG_tag.h"
#include "commons.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void TAG_bool_free(TAG_ptr_t ptr)
{
	TAG_bool_t *boolean = ptr.BOOL;
	free(boolean);

}

void TAG_bool_print(TAG_ptr_t ptr)
{
	char *str = (ptr.BOOL->boolean)? "true": "false";
	printf("%s\n", str);
}

static TAG_ptr_t TAG_init_bool(bool boolean)
{
	TAG_bool_t *ptr = calloc(1, sizeof(TAG_bool_t));
	ptr->boolean = boolean;
	TAG_ptr_t ret;
	ret.BOOL = ptr;
	return ret;
}

static bool TAG_get_bool(TAG_tag_t *tag)
{
	return tag->ptr.BOOL->boolean;
}

static TAG_bool_arg_t init_bool =
{
	.init = TAG_init_bool
};

static TAG_bool_arg_t get_bool =
{
	.get = TAG_get_bool
};


TAG_prototype_vtable_t TAG_bool_vtable =
{
	.free = TAG_bool_free,
	.print = TAG_bool_print,

	.size = 2,
	.fn =
	{
		[TAG_FN_BOOL_INIT].BOOL = &init_bool,
		[TAG_FN_BOOL_GET].BOOL = &get_bool,
	},
};