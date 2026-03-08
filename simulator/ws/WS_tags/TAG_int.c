#include "TAG_int.h"
#include "TAG_tag.h"
#include <stdio.h>

TAG_int_arg_t init_int;
TAG_int_arg_t get_int;
TAG_int_arg_t set_int;

TAG_ptr_t TAG_init_int(int64_t integer);
int64_t TAG_get_int(TAG_tag_t *tag);
void TAG_set_int(TAG_tag_t *tag, int64_t integer);

void TAG_int_free(TAG_ptr_t ptr)
{
	TAG_int_t *integer = ptr.INT;
	free(integer);
}
void TAG_int_print(TAG_ptr_t ptr, uint64_t tab)
{
	printf("%ld\n", ptr.INT->integer);
}

TAG_ptr_t TAG_int_copy(TAG_ptr_t ptr)
{
	return TAG_init_int(ptr.INT->integer);
}

TAG_ptr_t TAG_init_int(int64_t integer)
{
	TAG_int_t *ptr = calloc(1, sizeof(TAG_int_t));
	ptr->integer = integer;
	TAG_ptr_t ret;
	ret.INT = ptr;
	return ret;
}

int64_t TAG_get_int(TAG_tag_t *tag)
{
	return tag->ptr.INT->integer;
}

void TAG_set_int(TAG_tag_t *tag, int64_t integer)
{
	tag->ptr.INT->integer = integer;
}

TAG_int_arg_t init_int =
{
	.init = TAG_init_int
};

TAG_int_arg_t get_int =
{
	.get = TAG_get_int
};

TAG_int_arg_t set_int =
{
	.set = TAG_set_int
};


TAG_prototype_vtable_t TAG_int_vtable =
{
	.free = TAG_int_free,
	.print = TAG_int_print,
	.copy = TAG_int_copy,
	.size = 3,
	.fn =
	{
		[TAG_FN_INT_INIT].INT = &init_int,
		[TAG_FN_INT_GET].INT = &get_int,
		[TAG_FN_INT_SET].INT = &set_int
	},
};
