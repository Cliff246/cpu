#ifndef __DEVICE_VTABLE_HEADER__
#define __DEVICE_VTABLE_HEADER__

#include "WS_OBJ_object.h"
#include "token.h"
#include "WS_OBJ_port.h"
#include "WS_OBJ_flag.h"

typedef void *(*WS_MOD_object_init_fn_t)(OBJ_object_t obj);
//complete execution
typedef void (*WS_MOD_object_update_fn_t)(OBJ_object_t obj);
//to close off all reading
typedef bool (*WS_MOD_object_pull_fn_t)(OBJ_object_t obj);
//to close off all writing
typedef bool (*WS_MOD_object_push_fn_t)(OBJ_object_t obj);

typedef bool (*WS_MOD_object_get_fn_t)(OBJ_object_t obj);
typedef bool (*WS_MOD_object_set_fn_t)(OBJ_object_t obj);

typedef void (*WS_MOD_object_print_fn_t)(OBJ_object_t obj);
//should generate void *setting for config
typedef void (*WS_MOD_object_cmd_commit_fn_t)(OBJ_object_t obj);

typedef OBJ_port_handle_t (*WS_MOD_object_init_port_fn_t)(OBJ_object_t obj, OBJ_flag_t *flag);


typedef struct WS_MOD_vtable
{
	WS_MOD_object_init_fn_t init;
	WS_MOD_object_update_fn_t update;
	WS_MOD_object_pull_fn_t pull;
	WS_MOD_object_push_fn_t push;
	WS_MOD_object_print_fn_t print;

}MOD_vtable_t;

#endif
