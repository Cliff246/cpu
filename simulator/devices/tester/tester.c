#include "tester.h"


#include "OBJ_context.h"
#include "OBJ_resource.h"
#include "OBJ_hook.h"
#include "OBJ_global.h"
#include "OBJ_state.h"
#include "OBJ_local.h"

#include "MOD_description.h"
#include "MOD_symmap.h"


static MOD_symmap_t tester_map =
{
	.size = 1,
	.symbols =
	{
		[0] =
		{
			.key = "tester_hook",

			.hook = &tester_hook,
		}
	}
};

static MOD_description_t tester_desc =
{
	.id = 0,
	.version = 0,

	.dev_name = "tester",
	.dev_typeclass = "tester",
	.dl_name = "module_tester",
	.symmap = &tester_map,
};


const MOD_description_t *MOD_get_dev_desc(void)
{
	static bool initialized = false;
	printf("get mod dev desc\n");

	for(uint64_t i = 0; i < tester_desc.symmap->size; ++i)
	{
		MOD_sym_t *sym = &tester_desc.symmap->symbols[i];
		printf("key:%s ptr:%p\n", sym->key, sym->hook);
	}

	return &tester_desc;
}

void tester_hook_fn(OBJ_hnd_t *hnd, OBJ_context_t *context, OBJ_local_t *local, OBJ_global_t *global, OBJ_state_t *state)
{

}

char *tester_inputs[] =
{
	"basic_empty_hook_input"
};

char *tester_output[] =
{
	"basic_empty_hook_output"
};


OBJ_hook_t tester_hook =
{
	.inputs_size = 1,
	.inputs = (char **)tester_inputs,
	.outputs_size = 1,
	.outputs = (char **)tester_output,
	.key = "tester_hook",
	.local_size = 10,
	.OBJ_hook_fn = tester_hook_fn,
};