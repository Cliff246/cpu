#ifndef __DEVICE_VTABLE_HEADER__
#define __DEVICE_VTABLE_HEADER__

#include "OBJ_object.h"
#include "token.h"
#include "OBJ_port.h"
#include "OBJ_flag.h"

typedef void *(*MOD_object_init_fn_t)(OBJ_object_t obj);
//complete execution
typedef void (*MOD_object_update_fn_t)(OBJ_object_t obj);
//to close off all reading
typedef bool (*MOD_object_pull_fn_t)(OBJ_object_t obj);
//to close off all writing
typedef bool (*MOD_object_push_fn_t)(OBJ_object_t obj);

typedef bool (*MOD_object_get_fn_t)(OBJ_object_t obj);
typedef bool (*MOD_object_set_fn_t)(OBJ_object_t obj);

typedef void (*MOD_object_print_fn_t)(OBJ_object_t obj);
//should generate void *setting for config
typedef void (*MOD_object_cmd_commit_fn_t)(OBJ_object_t obj);

typedef OBJ_port_handle_t (*WS_MOD_object_init_port_fn_t)(OBJ_object_t obj, OBJ_flag_t *flag);


typedef struct WS_MOD_vtable
{
	MOD_object_init_fn_t init;
	MOD_object_update_fn_t update;
	MOD_object_pull_fn_t pull;
	MOD_object_push_fn_t push;
	MOD_object_print_fn_t print;

}MOD_vtable_t;

#endif
