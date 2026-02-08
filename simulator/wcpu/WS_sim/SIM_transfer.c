#include <stdio.h>
#include "SIM_transfer.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/*

void WS_simulator_print_bus_slot(WS_simulator_bus_slot_t *entry, int index)
{
	printf("slot:%d, start:%d, len:%d, dev_index:%d\n", index, entry->start, entry->len, entry->dev_index);
}

WS_simulator_bus_slot_t *WS_simulator_create_bus_slot(uint64_t base, uint64_t len, size_t dev_index)
{
	WS_simulator_bus_slot_t *entry =  calloc(1, sizeof(WS_simulator_bus_slot_t));
	assert(entry);
	entry->start = base;
	entry->len = len;
	entry->dev_index = dev_index;
	return entry;
}
*/


bool SIM_transfer_get_used(SIM_transfer_t *transfer)
{
	assert(transfer);
	return transfer->used;
}

//set the packet if used == false and sets used = true
//if return is true upaated packet and set used
//if return is false changed nothing
bool SIM_transfer_send(SIM_transfer_t *transfer, SIM_packet_t *packet)
{
	assert(transfer);

	if(SIM_transfer_get_used(transfer))
	{
		return false;
	}
	transfer->packet = packet;
	transfer->used = true;
	return true;

}


//reads the packet and sets used = false if used == true
//if read is success return true and change used = false
//if read is faile return false and nothing is writen
bool SIM_transfer_read(SIM_transfer_t *transfer, SIM_packet_t **packet)
{
	assert(transfer && packet);

	if(!SIM_transfer_get_used(transfer))
	{
		return false;
	}

	transfer->used = false;
	*packet = transfer->packet;
	return true;

}