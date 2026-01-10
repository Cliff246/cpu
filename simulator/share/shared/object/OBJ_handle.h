#ifndef __WS_OBJ_HANDLE_HEADER__
#define __WS_OBJ_HANDLE_HEADER__


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>



typedef struct WS_OBJ_hnd_utls
{
	
}OBJ_hnd_utls_t;

typedef struct WS_OBJ_handle
{
	uint8_t *state;
	uint64_t state_size;

	void *hnd;

}OBJ_hnd_t;

#endif