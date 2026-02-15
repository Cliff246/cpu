#ifndef __WS_OBJ_HANDLE_HEADER__
#define __WS_OBJ_HANDLE_HEADER__


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


//ownns subhandles in the object
typedef struct WS_OBJ_hnd
{
	uint16_t bundle_max_msgs;
	uint16_t msgs_size;
}OBJ_hnd_t;

#endif
