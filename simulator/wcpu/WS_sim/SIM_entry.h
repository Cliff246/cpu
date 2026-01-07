#ifndef __WS_SIM_ENTRY_HEADER__
#define __WS_SIM_ENTRY_HEADER__

#include "SIM_handle.h"
#include "SIM_port.h"
#include "WS_OBJ_object.h"
#include "hashmap.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct WS_SIM_entry
{
	OBJ_object_t object;
	SIM_handle_t handle;
	p_hashtable_t flags;
	uint64_t port_count;
	SIM_port_t *ports;


}SIM_entry_t;

//some start up behaviour fuck
SIM_entry_t *SIM_entry_init(SIM_handle_t hnd);



#endif
