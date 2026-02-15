#include "RUN_pools.h"
#include "RUN_packet.h"
#include <assert.h>
#include <string.h>
#include <stdint.h>

void RUN_init_pool_ring(RUN_pool_t *pool_ring, uint64_t size)
{

}

RUN_pkt_t *RUN_get_pkt_pool_ring(RUN_pool_t *pool, RUN_wireid_t id, uint16_t index)
{
	RUN_pool_ring_t *pool_ring = &pool->pool_ring;

	assert(id < pool_ring->elements_size);


	uint64_t base = pool_ring->elements[id].base;
	assert(pool_ring->elements[id].count > index);

	return &pool_ring->packets[base + index];

}

void RUN_set_pkt_pool_ring(RUN_pool_t *pool, RUN_wireid_t id, uint16_t index, RUN_pkt_t *pkt)
{

	RUN_pool_ring_t *pool_ring = &pool->pool_ring;

	assert(id < pool_ring->elements_size);

	uint64_t base = pool_ring->elements[id].base;
	assert(pool_ring->elements[id].count > index);


	memcpy(pool_ring->packets + base + index, pkt, sizeof(RUN_pkt_t));


}

//returns -1 on nothing, advances the round robin up one and sets the start and stop correctly
RUN_chnlid_t RUN_get_next_pool_rr(RUN_pool_t *pool, RUN_wireid_t wire_id)
{
	RUN_pool_rr_t *rr = &pool->pool_rr;

	RUN_pool_rr_owner_t *own = &rr->owners[wire_id];
	assert(own->id == wire_id);
	if(own->stop == own->start)
	{
		return -1;
	}
	uint64_t index = own->base + own->start;
	RUN_chnlid_t chnl_id = rr->roundrobin[index];
	own->start = (own->start + 1) % own->count;
	return chnl_id;
}
//adds the chnl to the round robin wire
void RUN_add_to_pool_rr(RUN_pool_t *pool, RUN_wireid_t wire_id, RUN_chnlid_t chnl_id)
{
	RUN_pool_rr_t *rr = &pool->pool_rr;

	RUN_pool_rr_owner_t *own = &rr->owners[wire_id];

	assert(own->id == wire_id);
	uint64_t offset = (own->stop + 1) % own->count;
	uint64_t index = own->base + own->stop;
	if(own->start == offset)
		return;
	rr->roundrobin[index] = chnl_id;




	own->stop = offset;
}


RUN_chnl_t *RUN_get_chnl_pool_chnl_obj(RUN_pool_t *pool, RUN_objid_t id, uint16_t index)
{
	
}
RUN_chnl_t *RUN_get_chnl_pool_chnl_wire(RUN_pool_t *pool, RUN_wireid_t id, uint16_t index)
{

}