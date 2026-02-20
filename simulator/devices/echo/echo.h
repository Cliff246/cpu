#ifndef __ECHO_HEADER__
#define __ECHO_HEADER__




#include "OBJ_handle.h"
#include "OBJ_hook.h"

extern OBJ_hook_t echo_hook_input;
extern OBJ_hook_t echo_hook_output;

void echo_hook_input_fn(OBJ_hnd_t *hnd, OBJ_context_t *context, OBJ_local_t *local, OBJ_global_t *global, OBJ_state_t *state);
void echo_hook_output_fn(OBJ_hnd_t *hnd, OBJ_context_t *context, OBJ_local_t *local, OBJ_global_t *global, OBJ_state_t *state);

#endif