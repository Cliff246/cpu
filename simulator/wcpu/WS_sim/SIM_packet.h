#ifndef __WS_SIM_PACKET_HEADER__
#define __WS_SIM_PACKET_HEADER__

#include "OBJ_message.h"
#include "SIM_commons.h"

#include <stdint.h>
#include <stdalign.h>

typedef struct WS_SIM_packet_raw
{
	_Alignas(8) uint8_t bytes[SIM_PACKET_SIZE];

}SIM_packet_raw_t;

typedef struct WS_SIM_packet_header
{
	OBJ_msgtype_t msgtype;
}SIM_packet_header_t;

typedef struct WS_SIM_packet_value
{
	OBJ_msgval_t val;
}SIM_packet_value_t;

typedef struct WS_SIM_packet_tag
{
	OBJ_msgtag_t tag;
}SIM_packet_tag_t;

typedef struct WS_SIM_packet_foot
{
	uint8_t bytes_used;
}SIM_packet_foot_t;

typedef enum WS_SIM_packet_type
{
	SIM_PACKET_INVAL,
	SIM_PACKET_RAW,
	SIM_PACKET_HEADER,
	SIM_PACKET_VALUE,
	SIM_PACKET_TAG,
	SIM_PACKET_FOOT,
}SIM_packet_type_t;

typedef union WS_SIM_packet_data
{
	SIM_packet_raw_t raw;
	SIM_packet_header_t header;
	SIM_packet_tag_t tag;
	SIM_packet_value_t value;
	SIM_packet_foot_t foot;
}SIM_packet_data_t;


typedef struct WS_SIM_packet
{
	_Alignas(8) SIM_packet_data_t data;
	SIM_packet_type_t type;
}SIM_packet_t;

#endif