#ifndef __WS_SIM_DEVICE_HEADER__
#define __WS_SIM_DEVICE_HEADER__

#include "MOD_description.h"
#include "SIM_channel.h"
#include "SIM_commons.h"
#include "CFG_prototag.h"
#include "CFG_entry.h"
#include "SIM_port.h"
#include "dynamic_lib.h"
#include "hashmap.h"
#include <stdint.h>


typedef struct WS_SIM_device
{
	p_hashtable_t tags;
	MOD_description_t *mod;
	SIM_port_t *port;
	SIM_channel_t *channels;
	uint64_t size;
}SIM_device_t;

void SIM_free_tag_table_elem(void *v);
bool SIM_prototag_add_to_device(SIM_device_t *device, CFG_prototag_t *tag);
static void SIM_device_create_channels(SIM_device_t *device);
static void SIM_device_create_tags(SIM_device_t *device, CFG_entry_t *entry);

void SIM_init_device(SIM_device_t *device, CFG_entry_t *entry);



SIM_channel_t *SIM_device_get_channel_by_wireid(SIM_device_t *device, uint64_t id);

//takes the full list of wires in the simulator
//changes the list with any connector being > 0 and no connector being 0
void SIM_device_contains_connector(SIM_device_t *device, int64_t *id, uint64_t size);
uint64_t SIM_device_get_connectors(SIM_device_t *device, int64_t *buf, uint64_t size);

void SIM_device_print(SIM_device_t *device);

#endif