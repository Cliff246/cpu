#include "TAG_int.h"

void TAG_int_free(TAG_ptr_t ptr)
{
	TAG_int_t *integer = ptr.INT;
	free(integer);
}

static TAG_ptr_t TAG_init_int(int64_t integer)
{
	TAG_int_t *ptr = calloc(1, sizeof(TAG_int_t));
	ptr->integer = integer;
	TAG_ptr_t ret;
	ret.INT = ptr;
	return ret;
}


static TAG_int_arg_t init_int =
{
	.init = TAG_init_int
};




TAG_prototype_vtable_t TAG_int_vtable =
{
	.free = TAG_int_free,
	.size = 1,
	.fn =
	{
		[TAG_FN_INT_INIT].INT = &init_int
	},
};
