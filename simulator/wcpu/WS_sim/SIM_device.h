#ifndef __WS_SIM_DEVICE_HEADER__
#define __WS_SIM_DEVICE_HEADER__

#include "MOD_description.h"
#include "RES_interface.h"
#include "RES_pool.h"
#include "SIM_commons.h"
#include "CFG_node.h"
#include "CFG_prototag.h"
#include "CFG_entry.h"
#include "SIM_handle.h"
#include "dynamic_lib.h"
#include "hashmap.h"


#include <stdint.h>
#include <stdbool.h>


typedef struct WS_SIM_device
{
	p_hashtable_t tags;
	//the preset tag assigned
	SIM_dtag_t tag;
	//the index of the device
	SIM_did_t id;
	SIM_handle_t *handle;
	RES_itrfc_t *interface;
}SIM_device_t;


void SIM_init_device(SIM_device_t *device, CFG_node_t *devctx);


static p_hashtable_t SIM_copy_hashtable(p_hashtable_t old);





//takes the full list of wires in the simulator
//changes the list with any connector being > 0 and no connector being 0
void SIM_device_contains_connector(SIM_device_t *device, int64_t *id, uint64_t size);
uint64_t SIM_device_get_connectors(SIM_device_t *device, int64_t *buf, uint64_t size);




void SIM_device_print(SIM_device_t *device);

#endif