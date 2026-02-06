#include "SIM_wirering.h"
#include "SIM_transfer.h"
#include <assert.h>
#include <stdlib.h>

void SIM_init_wirering(SIM_wirering_t *wirering, uint32_t size)
{
	SIM_transfer_t *ring =  calloc(1,  sizeof(SIM_transfer_t));
	assert(ring);
	wirering->ring = ring;
	wirering->size = size;
}


SIM_transfer_t SIM_wirering_read(SIM_wirering_t *wirering)
{

}

void SIM_wirering_write(SIM_wirering_t *wirering, SIM_transfer_t transfer)
{

}