#include "common.h"
#include "export.h"
#include "loader.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*

export_bundle_t *create_export_bundle(char *export)
{

	export_bundle_t *bundle = calloc(1, sizeof(export_bundle_t));

	bundle->file = create_sourcefile(export);


	return bundle;
}

#define PACKET_REG_STRING_SIZE 100
char *produce_register_packet(export_packet_t *packet)
{


	packet_register_t pckreg = packet->pck.pck_reg;


	char *result = calloc(PACKET_REG_STRING_SIZE + 1, sizeof(char));

	sprintf(result, "REG [%.2d] = %20ld", pckreg.regid, pckreg.value);

	return result;
}
#define PACKET_MEM_STRING_SIZE 50

char *produce_memory_packet(export_packet_t *packet)
{
	packet_memory_t pckmem = packet->pck.pck_mem;


	char *result = calloc(PACKET_MEM_STRING_SIZE + 1, sizeof(char));

	sprintf(result, "MEM [%.5d] = %20lld", pckmem.address, pckmem.value);

	return result;
}


export_packet_t create_packet_memory(int64_t value,int64_t address)
{

	packet_memory_t pckmem = {.address = address, .value = value};

	export_packet_t packet = {
		.func = produce_memory_packet,
		.type = PACKET_MEMORY,
		.pck.pck_mem = pckmem,

	};
	return packet;
}


export_packet_t create_packet_register(int regid, int file, int mode, int64_t value, export_mode_t outmode)
{

	packet_register_t pckreg = {.file = file, .regid = regid, .pmode = mode, .value = value, .mode = outmode};

	export_packet_t packet = {
		.func = produce_register_packet,
		.type = PACKET_REGISTER,
		.pck.pck_reg = pckreg,

	};
	return packet;
}

void add_packet_to_bundle(export_bundle_t *bnd, export_packet_t pck)
{
	if(bnd->packets_count >= MAXIMUM_PACKETS)
	{
		printf("too many packets\n");
		exit(1);
	}

	bnd->packets[bnd->packets_count++] = pck;

}

void export_dump(export_bundle_t *bnd)
{

	sourcefile_t *sf = bnd->file;
	bool success = open_sourcefile(sf);
	if(success == false)
	{
		printf("failed to dump file\n");
		return;
	}

	for(int i = 0; i < bnd->packets_count; ++i)
	{

		char *ptr = bnd->packets[i].func(&bnd->packets[i]);

		if(ptr == NULL)
		{
			continue;
		}
		write_sourcefile(sf, ptr);
		add_newline_sourcefile(sf);
		free(ptr);
	}


	close_sourcefile(sf);
}
*/