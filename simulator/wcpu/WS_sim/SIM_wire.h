#ifndef __WS_SIM_WIRE_HEADER__
#define __WS_SIM_WIRE_HEADER__

#include <SIM_channel.h>

#include <stdint.h>
#include <stdbool.h>

typedef struct WS_SIM_wire_config_end
{
	uint16_t oid;
	uint8_t cid;
}SIM_wire_config_end_t;

SIM_wire_config_end_t SIM_wire_config_end_init(uint16_t oid, uint8_t cid);



typedef struct WS_SIM_wire_config
{

	uint32_t delay;
	//this is the maximum amount a wire can hit the end
	uint8_t ends_size;
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
	uint8_t channel_length;
	//target channel
	uint8_t tgt_chnl;
	uint8_t src_chnl;
	uint32_t slot_start;
	uint32_t slot_length;

	//current index of the scroll limit
	uint32_t cur_scroll;
	uint32_t cur_len;

}SIM_wire_t;


SIM_wire_t SIM_wire_init(uint32_t channel_start, uint32_t channel_length, uint32_t slot_start, uint32_t slot_length);

void SIM_wire_update_scroll(SIM_wire_t *wire);


#endif
