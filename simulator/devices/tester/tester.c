#include "tester.h"

#include "MOD_description.h"
#include "OBJ_context.h"
#include "MOD_vtable.h"

static const MOD_vtable_t vtable =
{

};

static MOD_description_t tester_desc =
{
	.id = 0,
	.version = 0,

	.dev_name = "tester",
	.dev_typeclass = "tester",
	.dl_name = "module_tester",

	.vtable = &vtable,
	.extra = NULL,

};
