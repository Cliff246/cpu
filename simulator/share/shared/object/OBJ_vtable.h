#ifndef __WS_OBJ_VTABLE_HEADER__
#define __WS_OBJ_VTABLE_HEADER__

#include "OBJ_argument.h"
#include "OBJ_bundle.h"
typedef struct OBJ_vtable
{
	void (*OBJ_read)(OBJ_arg_t *arg, OBJ_bndl_t *bnd);
	void (*OBJ_update)(OBJ_arg_t *arg);
	void (*OBJ_write)(OBJ_arg_t *arg, OBJ_bndl_t *bnd);

}OBJ_vtable_t;

#endif