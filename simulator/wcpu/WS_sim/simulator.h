#ifndef __SIMULATOR_HEADER__
#define __SIMULATOR_HEADER__

#include "device.h"
#include "simulator_bus_slot.h"
#include "ws_sim_configure.h"
#include <stdlib.h>
#include <stdbool.h>




typedef struct wcpu_simulator
{
	WS_dev_t **dev_list;
	WS_simulator_bus_slot_t **bus_slot;
	size_t dev_count;
	size_t bus_slot_count;
}WS_simulator_t;


WS_dev_t *WS_simulator_get_device_from_id(WS_simulator_t *sim, WS_dev_id_t devid);
bool WS_simulator_get_device_from_address(WS_simulator_t *sim, WS_dev_t **dev, size_t address);

static void WS_simulator_sort_slots(WS_simulator_t *sim);

WS_simulator_t *WS_simulator_init();

//add device to simulator
void WS_simulator_add_device(WS_simulator_t *sim, WS_dev_t *dev);



//loads a group of devices from a config
bool WS_simulator_load_config(WS_simulator_t *sim, WS_cfg_file_t *config);



void WS_simulator_update(WS_simulator_t *sim);
void WS_simulator_print_slots(WS_simulator_t *sim);
void WS_simulator_print_all_devices(WS_simulator_t *sim);
//todo



#endif
