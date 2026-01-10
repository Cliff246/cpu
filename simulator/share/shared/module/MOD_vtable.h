#ifndef __DEVICE_VTABLE_HEADER__
#define __DEVICE_VTABLE_HEADER__

#include "token.h"
#include <stdbool.h>
#include <stdint.h>
#include "MOD_binding.h"
#include "MOD_function.h"
#include "MOD_polling.h"

//get if the
typedef MOD_polling_t (*MOD_get_polled_fn_t)(void);
//needs signifcantly more global data
typedef void (*MOD_user_input_fn_t)(const char *input);
typedef void (*MOD_status_checkin_fn_t)(void);
typedef int (*MOD_bind_count_fn_t)(void);
typedef const char *(*MOD_bind_name_fn_t)(int i);
typedef MOD_binding_type_t (*MOD_bind_type_fn_t)(int i);
typedef MOD_binding_t *(*MOD_bind_get_fn_t)(int i);

typedef MOD_function_t (*MOD_probe_fn_t)(int i);

typedef struct WS_MOD_vtable
{
	MOD_get_polled_fn_t polled;
	MOD_status_checkin_fn_t checkin;
	MOD_bind_count_fn_t bind_count;
	MOD_bind_name_fn_t bind_name;
	MOD_bind_type_fn_t bind_type;
	MOD_bind_get_fn_t bind_fn;
	MOD_probe_fn_t probe;

}MOD_vtable_t;


#endif
