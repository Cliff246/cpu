#ifndef __SIMULATOR_HEADER__
#define __SIMULATOR_HEADER__

#include "MANFST_manifest.h"
#include "IO_configure.h"
#include "MOD_description.h"
#include "CFG_context.h"
#include "RES_pool.h"
#include "RUN_graph.h"
#include "STAGE_stage.h"
#include "SCENE_scene.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


typedef struct WS_SIM_simulator
{
	CFG_context_t *ctx;
	STAGE_stage_t *stage;
	SCENE_scene_t *scene;
	
	RUN_graph_t *graph;
	RES_pool_t *pool;
}SIM_simulator_t;

// WS_dev_t *SIM_simulator_t_get_device_from_id(SIM_simulator_t_t *sim,
// WS_dev_id_t devid); bool
// SIM_simulator_get_device_from_address(SIM_simulator_t *sim, WS_dev_t **dev,
// size_t address);

// static void SIM_simulator_sort_slots(SIM_simulator_t *sim);

SIM_simulator_t *SIM_init_simulator(void);
bool SIM_alloc_graph(SIM_simulator_t *sim);
bool SIM_build_graph(SIM_simulator_t *sim);

bool SIM_init_stage_simulator(SIM_simulator_t *sim);

// add device to simulator

// advances a packet into from active forward
// loads a group of devices from a config
bool SIM_load_manifest_simulator(SIM_simulator_t *sim, MANFST_manifest_t *manifest);
void SIM_update_simulator(SIM_simulator_t *sim);
void SIM_simulator_print_slots(SIM_simulator_t *sim);
void SIM_simulator_print_all_devices(SIM_simulator_t *sim);
// todo

#endif
