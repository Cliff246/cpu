#ifndef __WS_SIM_DEVICE_HEADER__
#define __WS_SIM_DEVICE_HEADER__

#include "MOD_description.h"
#include "RES_interface.h"
#include "RES_pool.h"
#include "SIM_anchor.h"
#include "SIM_commons.h"
#include "CFG_node.h"
#include "MANFST_prototag.h"
#include "MANFST_entry.h"
#include "SIM_handle.h"
#include "dynamic_lib.h"
#include "hashmap.h"
#include "SIM_port.h"

#include <stdint.h>
#include <stdbool.h>



typedef struct SIM_device
{
	p_hashtable_t tags;
	//the preset tag assigned
	SIM_dkey_t dkey;
	//the index of the device
	SIM_did_t id;
	SIM_handle_t *handle;
	RES_itrfc_t *interface;

	SIM_port_t port;

}SIM_device_t;

//locals to the device
static p_hashtable_t SIM_copy_hashtable(p_hashtable_t old);


//init stage

SIM_device_t *SIM_alloc_device();
void SIM_init_device(SIM_device_t *device, CFG_node_t *devctx);
void SIM_resolve_device(SIM_device_t *device);
void SIM_build_device(SIM_device_t *device);


//free(not done yet)
void SIM_free_device(SIM_device_t *device);



void SIM_device_print(SIM_device_t *device);

#endif