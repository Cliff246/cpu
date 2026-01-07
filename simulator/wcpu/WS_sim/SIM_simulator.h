#ifndef __SIMULATOR_HEADER__
#define __SIMULATOR_HEADER__

#include "simulator_bus_slot.h"
#include "ws_sim_configure.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "SIM_entry.h"
#include "SIM_wire.h"
#include "SIM_packet.h"

typedef struct WS_SIM_entry_alias
{
	uint64_t index;
	SIM_entry_t *alias;
}SIM_entry_alias_t;

//I want to use a flat buffer for all the wire channels to avoid cache thrashing
//
typedef struct WS_SIM_graph
{


	//the entries in the graph
	uint64_t aliases_size;
	SIM_entry_alias_t *aliases;


	uint64_t wires_size;
	//all the wrires in the
	SIM_wire_t *wires;

	//SUM(wires->wire
	uint64_t wirechannels_size;
	//flat buffer of all wire channels
	//this lets us know what port a wire connects to and what object
	SIM_wirechannel_t *wirechannels;

	bool built;
}SIM_graph_t;

SIM_graph_t *SIM_graph_init();

//should be cleaned every single frame and refilled
typedef struct WS_SIM_active_packets
{
	//should be at most the size of wires_size
	uint64_t alloc;
	uint64_t size;
	uint64_t *active;
}SIM_active_packets_t;

//all the packets that are defined in the system
typedef struct WS_SIM_live_packets
{
	uint64_t alloc;
	uint64_t size;
	SIM_packet_t *live;
}SIM_live_packets_t;


//this should be a flat list of ports that are active in each entry
typedef struct WS_SIM_ports_active
{

}SIM_ports_active_t;

typedef struct WS_SIM_simulator
{
	int sid;
	SIM_graph_t *graph;
	SIM_active_packets_t *active_packets;
	SIM_live_packets_t *live_packets;

	uint64_t entries_size;
	SIM_entry_t *entries;


	size_t dev_count;
	size_t bus_slot_count;
}SIM_simulator_t;


//WS_dev_t *SIM_simulator_t_get_device_from_id(SIM_simulator_t_t *sim, WS_dev_id_t devid);
//bool SIM_simulator_get_device_from_address(SIM_simulator_t *sim, WS_dev_t **dev, size_t address);

//static void SIM_simulator_sort_slots(SIM_simulator_t *sim);

SIM_simulator_t *SIM_simulator_init(void);

void SIM_simulator_rebuild_graph(SIM_simulator_t *sim);

//add device to simulator
void SIM_simulator_add_entry(SIM_simulator_t *sim,  SIM_entry_t entry);



//loads a group of devices from a config
bool SIM_simulator_load_config(SIM_simulator_t *sim, WS_cfg_file_t *config);



void SIM_simulator_update(SIM_simulator_t *sim);
void SIM_simulator_print_slots(SIM_simulator_t *sim);
void SIM_simulator_print_all_devices(SIM_simulator_t *sim);
//todo



#endif
