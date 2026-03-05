#include "echo.h"


#include "OBJ_context.h"
#include "OBJ_resource.h"
#include "OBJ_hook.h"
#include "OBJ_global.h"
#include "OBJ_state.h"
#include "OBJ_local.h"

#include "MOD_description.h"
#include "MOD_symmap.h"
#include <stdio.h>


static MOD_symmap_t echo_map =
{
	.size = 2,
	.symbols =
	{
		[0] =
		{
			.key = "echo_input",

			.hook = &echo_hook_input,
		},
		[1] =
		{
			.key = "echo_output",

			.hook = &echo_hook_output,
		}
	}
};

static MOD_description_t echo_desc =
{
	.id = 0,
	.version = 0,

	.dev_name = "echo_desc",
	.dev_typeclass = "simple",
	.dl_name = "dev_echo",
	.symmap = &echo_map,
};


const MOD_description_t *MOD_get_dev_desc(void)
{
	static bool initialized = false;
	printf("get mod dev desc\n");

	for(uint64_t i = 0; i < echo_desc.symmap->size; ++i)
	{
		MOD_sym_t *sym = &echo_desc.symmap->symbols[i];
		printf("key:%s ptr:%p\n", sym->key, sym->hook);
	}

	return &echo_desc;
}


void echo_hook_input_fn(OBJ_hnd_t *hnd, OBJ_context_t *context, OBJ_local_t *local, OBJ_global_t *global, OBJ_state_t *state)
{
	printf("\n\n\nthis is the echo hook function\n\n\n");
}

char *echo_input_hook_inputs[] =
{
	"busin",
	"echo_resource"
};

char *echo_input_hook_outputs[] =
{
	"echo_resource"
};

/*
//	input hook
//
*/

OBJ_hook_t echo_hook_input =
{
	.inputs_size = 2,
	.inputs = (char **)echo_input_hook_inputs,
	.outputs_size = 1,
	.outputs = (char **)echo_input_hook_outputs,
	.key = "echo_input",
	.local_size = 10,
	.OBJ_hook_fn = echo_hook_input_fn,
};


void echo_hook_output_fn(OBJ_hnd_t *hnd, OBJ_context_t *context, OBJ_local_t *local, OBJ_global_t *global, OBJ_state_t *state)
{

}

char *echo_output_hook_inputs[] =
{
	"echo_resource",
	"busout"
};



char *echo_output_hook_outputs[] =
{
	"busout"
};

OBJ_hook_t echo_hook_output =
{
	.inputs_size = 2,
	.inputs = (char **)echo_output_hook_inputs,
	.outputs_size = 1,
	.outputs = (char **)echo_output_hook_outputs,
	.key = "echo_output",
	.local_size = 10,
	.OBJ_hook_fn = echo_hook_output_fn,
};
