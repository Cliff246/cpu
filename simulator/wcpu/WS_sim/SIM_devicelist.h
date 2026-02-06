#ifndef __WS_SIM_DEVICE_LIST_HEADER__
#define __WS_SIM_DEVICE_LIST_HEADER__

#include "CFG_entry.h"
#include "SIM_device.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct WS_SIM_devicelist
{
	uint64_t size;
	SIM_device_t *devices;
	bool done;
}SIM_devicelist_t;


SIM_devicelist_t *SIM_init_devicelist(uint64_t size);

SIM_device_t *SIM_get_device_devicelist(SIM_devicelist_t *devlist, uint64_t i);

bool SIM_devicelist_done(SIM_devicelist_t *devlist);

uint64_t SIM_get_devicelist_size(SIM_devicelist_t *devlist);

void SIM_devicelist_print_all(SIM_devicelist_t *devlist);

#endif