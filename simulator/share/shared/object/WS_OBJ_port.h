#ifndef __WS_OBJ_PORT_HEADER__
#define __WS_OBJ_PORT_HEADER__

#include "WS_OBJ_object.h"
#include "WS_OBJ_message.h"

#include <stdint.h>
#include <stdbool.h>

#define OBJ_PORT_HANDLE_NAME_LENGTH 20

typedef struct WS_OBJ_port OBJ_port_t;


typedef void (* WS_OBJ_object_port_intake_fn_t)(OBJ_object_t obj, OBJ_port_t *port, OBJ_message_t msg);
typedef OBJ_message_t (* WS_OBJ_object_port_extake_fn_t )(OBJ_object_t obj, OBJ_port_t *port);

struct WS_OBJ_port
{
	const int hnd;
	const int type;
	const WS_OBJ_object_port_intake_fn_t intake;
	const WS_OBJ_object_port_extake_fn_t extake;
};



typedef bool (* WS_OBJ_object_port_write_fn_t)(OBJ_object_t obj, OBJ_port_t *port);
typedef bool ( *WS_OBJ_object_port_read_fn_t)(OBJ_object_t obj, OBJ_port_t *port);


typedef struct WS_OBJ_port_handle
{
	const WS_OBJ_object_port_write_fn_t write;
	const WS_OBJ_object_port_read_fn_t read;
	const int id;
	const int type;
	char name[OBJ_PORT_HANDLE_NAME_LENGTH];

}OBJ_port_handle_t;


#endif