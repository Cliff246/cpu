#ifndef __WS_SIM_WIRERING_HEADER__
#define __WS_SIM_WIRERING_HEADER__

#include "SIM_channel.h"
#include "SIM_packetbuffer.h"
#include "SIM_transfer.h"
#include <stdint.h>


typedef struct WS_SIM_wirering
{
	uint32_t size;
	uint32_t index;
	SIM_transfer_t *ring;
}SIM_wirering_t;


void SIM_init_wirering(SIM_wirering_t *wirering, uint32_t size);

SIM_transfer_t SIM_wirering_read(SIM_wirering_t *wirering);
void SIM_wirering_write(SIM_wirering_t *wirering, SIM_transfer_t transfer);



#endif