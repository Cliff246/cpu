#ifndef __WS_OBJ_FUNCTION_HEADER__
#define __WS_OBJ_FUNCTION_HEADER__

#include "OBJ_context.h"
#include "OBJ_handle.h"

#define OBJ_FN_KEY_SIZE 10

typedef void (*OBJ_tmplfn_t)(OBJ_hnd_t *hnd, OBJ_ctx_t *ctx);




typedef struct WS_OBJ_function
{
	char key[OBJ_FN_KEY_SIZE];
	OBJ_tmplfn_t tmplfn;
}OBJ_fn_t;



#endif