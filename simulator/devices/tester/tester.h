#ifndef __WS_TESTER_HEADER__
#define __WS_TESTER_HEADER__


#include "OBJ_handle.h"
#include "OBJ_hook.h"

extern OBJ_hook_t tester_hook;

void tester_hook_fn(OBJ_hnd_t *hnd, OBJ_context_t *context, OBJ_local_t *local, OBJ_global_t *global, OBJ_state_t *state);

#endif