#ifndef __OBJ_HOOK_HEADER__
#define __OBJ_HOOK_HEADER__

#include "OBJ_context.h"
#include "OBJ_handle.h"
#include "OBJ_local.h"
#include "OBJ_global.h"
#include "OBJ_state.h"
#include <stdint.h>

//symbol resoves features
//resources are binded and operated on given the functions local scope
//the system parses -> input to output determining all resources and their constraints
typedef struct OBJ_hook
{
	char *key;
	uint64_t inputs_size;
	char **inputs;
	uint64_t outputs_size;
	char **outputs;
	uint64_t local_size;

	void (*OBJ_hook_fn)(OBJ_hnd_t *hnd, OBJ_context_t *context, OBJ_local_t *local, OBJ_global_t *global, OBJ_state_t *state);
}OBJ_hook_t;

#endif