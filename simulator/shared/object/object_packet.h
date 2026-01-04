#ifndef __OBJECT_PACKET_HEADER__
#define __OBJECT_PACKET_HEADER__

#include <stdint.h>
#include <stdbool.h>

typedef struct WS_OBJ_packet_read
{
	uint64_t address;
}OBJ_packet_read_t;

typedef struct WS_OBJ_packet_write
{
	uint64_t address;
	uint64_t value;
}OBJ_packet_write_t;

typedef struct WS_OBJ_packet_command
{
	uint64_t command;
}OBJ_packet_command_t;

typedef struct WS_OBJ_packet_interupt
{
	uint64_t type;
}OBJ_packet_interupt_t;

typedef enum WS_OBJ_packet_type
{
	OBJ_PACKET_READ,
	OBJ_PACKET_WRITE,
	OBJ_PACKET_COMMAND,
	OBJ_PACKET_INTERUPT,
}OBJ_packet_type_t;

typedef union WS_OBJ_packet_union
{
	OBJ_packet_read_t read;
	OBJ_packet_write_t write;
	OBJ_packet_command_t command;
	OBJ_packet_interupt_t interupt;
}OBJ_packet_union_t;

typedef struct WS_OBJ_packet
{

	OBJ_packet_type_t type;
	OBJ_packet_union_t value;
}OBJ_packet_t;

#endif