#ifndef __EXPORT_HEADER__
#define __EXPORT_HEADER__

#include <stdint.h>
#include "loader.h"

typedef enum packet_type
{
	PACKET_UNKNOWN,
	PACKET_REGISTER,
	PACKET_MEMORY,
	PACKET_PROGRAM,
}packet_type_t;

struct export_packet;

typedef char * (*packet_string_fn)(struct export_packet *);

typedef enum export_mode
{
	EXPORT_MODE_INVAL,
	EXPORT_MODE_INT,
	EXPORT_MODE_UINT,
	EXPORT_MODE_FLOAT,
	EXPORT_MODE_DOUBLE,
	EXPORT_MODE_BINARY,
	EXPORT_MODE_CHAR,
	EXPORT_MODE_STRING,
	EXPORT_MODE_HEX,

}export_mode_t;

typedef struct packet_register
{
	int regid;
	int file;
	int pmode;
	int64_t value;
	export_mode_t mode;

}packet_register_t;

typedef struct packet_memory
{
	int64_t value;
	int64_t address;


}packet_memory_t;


typedef struct export_packet
{
	packet_type_t type;
	packet_string_fn func;
	union
	{
		packet_memory_t pck_mem;
		packet_register_t pck_reg;
	}pck;
}export_packet_t;

#define MAXIMUM_PACKETS 300

typedef struct export_bundle
{
	export_packet_t packets[MAXIMUM_PACKETS];
	int packets_count;
	sourcefile_t *file;
}export_bundle_t;


export_packet_t create_packet_memory(int64_t value,int64_t address);
export_packet_t create_packet_register(int regid, int file, int mode, int64_t value, export_mode_t outmode);

export_bundle_t *create_export_bundle(char *export);

void add_packet_to_bundle(export_bundle_t *bnd, export_packet_t pck);


void export_dump(export_bundle_t *bnd);




#endif