#ifndef __WS_OBJ_NODE_HEADER__
#define __WS_OBJ_NODE_HEADER__

#include <stdbool.h>
#include <stdint.h>
#include <OBJ_handle.h>
#include <OBJ_function.h>

#define OBJ_NODE_KEY_SIZE 8

typedef struct WS_OBJ_node
{
	char key[OBJ_NODE_KEY_SIZE];
	OBJ_fn_t fn;

}OBJ_node_t;

#endif