#ifndef __WS_OBJ_FUNCTION_HEADER__
#define __WS_OBJ_FUNCTION_HEADER__

#include "OBJ_context.h"
#include "OBJ_handle.h"

#define OBJ_FN_KEY_SIZE 10

//object is the global
//hnd is the local stage data
//ctx is the context of the data in
//ctx is the context of the data out
typedef void (*OBJ_tmpl_fn_t)(OBJ_hnd_t *hnd, OBJ_ctx_t *ctx);

typedef struct WS_OBJ_function
{
	OBJ_tmpl_fn_t tmplfn;
	char key[OBJ_FN_KEY_SIZE];
}OBJ_fn_t;



#endif