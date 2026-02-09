#ifndef __WS_SIM_DEVICE_HEADER__
#define __WS_SIM_DEVICE_HEADER__

#include "MOD_description.h"
#include "SIM_channel.h"
#include "SIM_commons.h"
#include "SIM_devcfg.h"
#include "CFG_prototag.h"
#include "CFG_entry.h"
#include "SIM_handle.h"
#include "SIM_port.h"
#include "dynamic_lib.h"
#include "hashmap.h"


#include <stdint.h>
#include <stdbool.h>


typedef struct WS_SIM_device
{
	p_hashtable_t tags;
	SIM_port_t *port;
	SIM_dtag_t dtag;
	SIM_handle_t *handle;

}SIM_device_t;


void SIM_init_device(SIM_device_t *device, SIM_devcfg_t *devctx);







//takes the full list of wires in the simulator
//changes the list with any connector being > 0 and no connector being 0
void SIM_device_contains_connector(SIM_device_t *device, int64_t *id, uint64_t size);
uint64_t SIM_device_get_connectors(SIM_device_t *device, int64_t *buf, uint64_t size);




void SIM_device_print(SIM_device_t *device);

#endif