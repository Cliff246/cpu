#include "TAG_string.h"
#include "TAG_prototype.h"

void TAG_string_free(TAG_ptr_t ptr)
{
	TAG_string_t *string = ptr.STRING;

	free(string->string);
}

TAG_prototype_vtable_t TAG_string_vtable =
{
	.free = TAG_string_free
};