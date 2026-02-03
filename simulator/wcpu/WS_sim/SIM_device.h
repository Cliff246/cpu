#ifndef __WS_SIM_DEVICE_HEADER__
#define __WS_SIM_DEVICE_HEADER__

#include "MOD_description.h"
#include "SIM_commons.h"
#include "CFG_prototag.h"
#include "CFG_entry.h"
#include "dynamic_lib.h"
#include "hashmap.h"
#include <stdint.h>


typedef struct WS_SIM_device
{
	uint16_t oid;
	//needs to be resizable
	p_hashtable_t tags;
	MOD_description_t *mod;
}SIM_device_t;

void SIM_free_tag_table_elem(void *v);
bool SIM_prototag_add_to_device(SIM_device_t *device, CFG_prototag_t *tag);
SIM_device_t *SIM_init_device(CFG_entry_t *entry);


//takes the full list of wires in the simulator
//changes the list with any connector being > 0 and no connector being 0
void SIM_device_contains_connector(SIM_device_t *device, int64_t *id, uint64_t size);
uint64_t SIM_device_get_connectors(SIM_device_t *device, int64_t *buf, uint64_t size);

#endif