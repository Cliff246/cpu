#ifndef __WS_SIM_WIRE_HEADER__
#define __WS_SIM_WIRE_HEADER__

#include <SIM_entry.h>
#include <SIM_packet.h>
#include <SIM_channel.h>

#include <stdint.h>
#include <stdbool.h>

typedef struct WS_SIM_wire_config_end
{
	SIM_entry_t *entry;
	uint32_t port;
}SIM_wire_config_end_t;

SIM_wire_config_end_t SIM_wire_config_end_init(SIM_entry_t *entry, uint32_t port_id);



typedef struct WS_SIM_wire_config
{

	uint32_t delay;
	uint32_t ends_size;
	SIM_wire_config_end_t *ends;
	struct
	{
		bool set;
	}flags;
}SIM_wire_config_t;

SIM_wire_config_t *SIM_wire_config_init(uint32_t delay);

void SIM_wire_config_add_end(SIM_wire_config_t *wire_config, SIM_wire_config_end_t end);

bool SIM_wire_config_set(SIM_wire_config_t *wire_config);


//contains wire information, number of slots, base state, etc
typedef struct WS_SIM_wire
{

	//designed for channels, shows where the wire can end up
	uint32_t channel_start;
	uint32_t channel_length;

	uint32_t slot_start;
	uint32_t slot_length;

	//current index of the scroll limit
	uint32_t current_scroll;


}SIM_wire_t;


SIM_wire_t SIM_wire_init(uint32_t channel_start, uint32_t channel_length, uint32_t slot_start, uint32_t slot_length);

void SIM_wire_update_scroll(SIM_wire_t *wire);


#endif
