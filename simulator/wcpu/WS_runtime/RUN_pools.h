#ifndef __RUN_POOL_HEADER__
#define __RUN_POOL_HEADER__

#include "RUN_channel.h"
#include "RUN_commons.h"
#include "RUN_object.h"
#include "RUN_packet.h"
#include "SIM_stage.h"
#include <stdint.h>

typedef struct RUN_pool RUN_pool_t;
//indexed via RUN_wireid
//
typedef struct RUN_pool_ring_element
{
	uint32_t base;
	uint32_t start;
	RUN_wireid_t id;
	uint16_t count;
	uint16_t stop;


}RUN_pool_ring_elm_t;

typedef struct RUN_pool_ring
{


	uint64_t elements_size;
	RUN_pool_ring_elm_t *elements;
	uint64_t packets_size;
	RUN_pkt_t *packets;
}RUN_pool_ring_t;
//TODO

//the packet ring's for wires pool
void RUN_init_pool_ring(RUN_pool_t *pool, const SIM_stage_t *stage);

//getst the index of a packet raw
RUN_pkt_t *RUN_get_pkt_pool_ring(RUN_pool_t *pool, RUN_wireid_t id, uint16_t index);
//sets
void RUN_set_pkt_pool_ring(RUN_pool_t *pool, RUN_wireid_t id, uint16_t index, RUN_pkt_t *pkt);




//round robin holder of wire information
typedef struct RUN_pool_roundrobin_owner
{
	//the base index
	uint32_t base;
	//total size
	uint16_t count;
	//id to compare too
	RUN_wireid_t id;
	//start of the round robin
	uint16_t start;
	//stop of the round robin
	uint16_t stop;

}RUN_pool_rr_owner_t;

typedef struct RUN_pool_roundrobin
{
	uint64_t owner_size;
	RUN_pool_rr_owner_t *owners;
	uint64_t roundrobin_size;
	RUN_chnlid_t *roundrobin;

}RUN_pool_rr_t;

//TODO
void RUN_init_pool_rr(RUN_pool_t *pool, const SIM_stage_t *stage);

//returns -1 on nothing, advances the round robin up one and sets the start and stop correctly
RUN_chnlid_t RUN_get_next_pool_rr(RUN_pool_t *pool, RUN_wireid_t wire_id);

//adds the chnl to the round robin wire
void RUN_add_to_pool_rr(RUN_pool_t *pool, RUN_wireid_t wire_id, RUN_chnlid_t chnl_id);

typedef struct RUN_pool_channel_obj_element
{
	//base of the obj aliases
	uint32_t base;
	//count of the obj aliases
	uint16_t count;
	//id to compare too
	RUN_objid_t id;
}RUN_pool_chnl_obj_elm_t;

typedef struct RUN_pool_channel_wire_element
{
	//base of the wire aliases
	uint32_t base;
	//count of the wire aliases
	uint16_t count;
	//id to compare too
	RUN_wireid_t id;
}RUN_pool_chnl_wire_elm_t;


//the central channel container
typedef struct RUN_pool_channel
{
	//size of the obj_aliases should be == to everything else
	uint64_t obj_aliases_size;
	//a list of aliases of an object to the channel
	RUN_pool_chnl_obj_elm_t *obj_aliases;
	//size of the wire_aliases should be == to everything else
	uint64_t wire_aliases_size;
	//a list of aliases of a wire to the channel
	RUN_pool_chnl_wire_elm_t *wire_aliases;
	//total channels size
	uint64_t chnls_size;
	//a list of the channels
	RUN_chnl_t *chnls;
}RUN_pool_chnl_t;

void RUN_init_pool_chnl(RUN_pool_t *pool, const SIM_stage_t *stage);

RUN_chnl_t *RUN_get_chnl_pool_chnl_obj(RUN_pool_t *pool, RUN_objid_t id, uint16_t index);
RUN_chnl_t *RUN_get_chnl_pool_chnl_wire(RUN_pool_t *pool, RUN_wireid_t id, uint16_t index);


//the pool of all objects
typedef struct RUN_pool_object
{
	uint64_t objs_size;
	RUN_obj_t *objs;
}RUN_pool_obj_t;

//TODO
void RUN_init_pool_obj(RUN_pool_t *pool, const SIM_stage_t *stage);

//the central pool for all runtime components
typedef struct RUN_pool
{
	bool allocated;
    uint8_t *arena;
    uint64_t arena_size;
    uint64_t arena_offset;

	RUN_pool_chnl_t pool_chnl;
	RUN_pool_ring_t pool_ring;
	RUN_pool_rr_t pool_rr;
	RUN_pool_obj_t pool_obj;

}RUN_pool_t;

//pool section
void RUN_alloc_pool(RUN_pool_t *pool);
void RUN_build_pool(RUN_pool_t *pool, const SIM_stage_t *stage);

void *RUN_alloc_arena_pool(RUN_pool_t *pool, uint64_t size, uint64_t align);



#endif