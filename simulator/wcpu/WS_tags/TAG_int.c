#include "TAG_int.h"
#include <stdio.h>

void TAG_int_free(TAG_ptr_t ptr)
{
	TAG_int_t *integer = ptr.INT;
	free(integer);
}

void TAG_int_print(TAG_ptr_t ptr)
{
	printf("%ld\n", ptr.INT->integer);
}

static TAG_ptr_t TAG_init_int(int64_t integer)
{
	TAG_int_t *ptr = calloc(1, sizeof(TAG_int_t));
	ptr->integer = integer;
	TAG_ptr_t ret;
	ret.INT = ptr;
	return ret;
}

static int64_t TAG_get_int(TAG_tag_t *tag)
{
	return tag->ptr.INT->integer;
}

static TAG_int_arg_t init_int =
{
	.init = TAG_init_int
};

static TAG_int_arg_t get_int =
{
	.get = TAG_get_int
};



TAG_prototype_vtable_t TAG_int_vtable =
{
	.free = TAG_int_free,
	.print = TAG_int_print,

	.size = 2,
	.fn =
	{
		[TAG_FN_INT_INIT].INT = &init_int,
		[TAG_FN_INT_GET].INT = &get_int,
	},
};
