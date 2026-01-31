#ifndef __WS_SIM_DEVICE_HEADER__
#define __WS_SIM_DEVICE_HEADER__

#include "SIM_commons.h"
#include "SIM_prototag.h"
#include "hashmap.h"
#include <stdint.h>


typedef struct WS_SIM_device
{
	uint16_t oid;
	//needs to be resizable
	p_hashtable_t tags;

}SIM_device_t;

void SIM_free_tag_table_elem(void *v);
bool SIM_prototag_add_to_device(SIM_device_t *device, SIM_prototag_t *tag);
SIM_device_t *SIM_init_device(SIM_prototag_t **tags, uint64_t count);


#endif