#ifndef __WS_SIM_WIRESLOT_HEADER__
#define __WS_SIM_WIRESLOT_HEADER__

#include <stdint.h>
#include <stdbool.h>

#include "OBJ_message.h"
#include "SIM_packet.h"


typedef struct WS_SIM_transfer
{
	//packet index
	SIM_packet_t packet;
	bool used;

}SIM_transfer_t;

bool SIM_transfer_get_used(SIM_transfer_t *transfer);

//set the packet if used == false and sets used = true
//if return is true upaated packet and set used
//if return is false changed nothing
bool SIM_transfer_send(SIM_transfer_t *transfer, SIM_packet_t packet);


//reads the packet and sets used = false if used == true
//if read is success return true and change used = false
//if read is faile return false and nothing is writen
bool SIM_transfer_read(SIM_transfer_t *transfer, SIM_packet_t *packet);


#endif
