#ifndef __SIMULATOR_HEADER__
#define __SIMULATOR_HEADER__

#include "IO_configure.h"
#include "MOD_description.h"
#include "SIM_transfer.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "SIM_graph.h"
#include "SIM_wire.h"

typedef struct WS_SIM_simulator
{
  int sid;

  SIM_graph_t *graph;

} SIM_simulator_t;

// WS_dev_t *SIM_simulator_t_get_device_from_id(SIM_simulator_t_t *sim,
// WS_dev_id_t devid); bool
// SIM_simulator_get_device_from_address(SIM_simulator_t *sim, WS_dev_t **dev,
// size_t address);

// static void SIM_simulator_sort_slots(SIM_simulator_t *sim);

SIM_simulator_t *SIM_simulator_init(void);

void SIM_simulator_rebuild_graph(SIM_simulator_t *sim);

// add device to simulator

// advances a packet into from active forward
// loads a group of devices from a config
bool SIM_simulator_load_config(SIM_simulator_t *sim, WS_cfg_file_t *config);

void SIM_simulator_update(SIM_simulator_t *sim);
void SIM_simulator_print_slots(SIM_simulator_t *sim);
void SIM_simulator_print_all_devices(SIM_simulator_t *sim);
// todo

#endif
