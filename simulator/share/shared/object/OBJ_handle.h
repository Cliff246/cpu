#ifndef __WS_OBJ_HANDLE_HEADER__
#define __WS_OBJ_HANDLE_HEADER__


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int OBJ_hid_t;

//ownns subhandles in the object
typedef struct WS_OBJ_handle
{
	//handle id in the object
	OBJ_hid_t hid;

	size_t memsize;
	uint8_t *mem;
}OBJ_hnd_t;

#endif