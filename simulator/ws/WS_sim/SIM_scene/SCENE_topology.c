#include "SCENE_topology.h"
#include "SCENE_link.h"
#include "SCENE_scope.h"
#include "SCENE_context.h"
#include "SCENE_port.h"
#include "SIM_commons.h"
#include "commons.h"
#include "hashmap.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>




//---------------------------
//protosypes
//---------------------------

struct port_uid
{
	SIM_uid_t port_uid;
};

void free_linkset(void *linkset);
void free_port_uid(void *port_uid);

struct port_uid *init_port_uid(SIM_uid_t uid);
//-------------------------------
//implements
//-------------------------------



void free_linkset(void *linkset)
{
	return;
}

void free_port_uid(void *port_uid)
{
	free(port_uid);
}

struct port_uid *init_port_uid(SIM_uid_t uid)
{
	struct port_uid *port_uid = calloc(1, sizeof(struct port_uid));
	port_uid->port_uid = uid;
	return port_uid;
}

//--------------------------------------------
//commons
//--------------------------------------------

SCENE_topology_t *SCENE_init_topology(void)
{
	SCENE_topology_t *topology = calloc(1, sizeof(SCENE_topology_t));


	topology->links_alloc = 100;
	topology->links_count = 0;
	topology->ports_alloc = 10;
	topology->ports_count = 0;
	topology->ports_list = calloc(topology->ports_alloc, sizeof(SCENE_port_t *));
	topology->links_list = calloc(topology->links_alloc, sizeof(SCENE_link_t *));
	return topology;

}
uint64_t SCENE_append_link_topology(SCENE_topology_t *topology, SCENE_link_t *link)
{

	if(topology->links_alloc <= topology->links_count)
	{
		topology->links_alloc = (topology->links_alloc + 1) * 2;
		topology->links_list = realloc_safe(topology->links_list, topology->links_alloc, sizeof(SCENE_link_t *));
	}
	uint64_t pos = topology->links_count++;
	topology->links_list[pos] = link;

	return pos;
}

uint64_t SCENE_append_port_topology(SCENE_topology_t *topology, SCENE_port_t *port)
{
	if(topology->ports_alloc <= topology->ports_count)
	{
		topology->ports_alloc = (topology->ports_alloc + 1) * 2;
		topology->ports_list = realloc_safe(topology->ports_list, topology->ports_alloc, sizeof(SCENE_port_t *));
	}
	uint64_t pos = topology->ports_count++;
	topology->ports_list[pos] = port;

	return pos;
}


void SCENE_finalize_links_topology(SCENE_topology_t *topology)
{
	topology->links_finished = true;
}


void SCENE_symbolize_topology(SCENE_topology_t *topology, SCENE_context_t *context)
{
	if(topology->links_finished == false)
	{
		assert(0 && "links finished is false");
		exit(EXIT_FAILURE);
	}

	p_hashtable_t table_negative = new_hash_table(topology->links_count * 3, free_port_uid);
	//printf("%x\n", 's');
	for(uint64_t j = 0; j < topology->ports_count; ++j)
	{

		SCENE_port_t *port = topology->ports_list[j];

		for(uint64_t port_negative = 0; port_negative < port->channels_count; ++port_negative)
		{
			char *negative = port->negative[port_negative];

			if(getdata_from_hash_table(table_negative, negative) != NULL)
			{
	//			printf("%s\n", negative);
				assert(0 && "already had key in table");
			}
			struct port_uid *port_uid = init_port_uid(port->uid);
			addto_hash_table(table_negative, negative, port_uid);
		}


	}




	uint64_t count = SCENE_get_count_scope(context->scope);

	for(uint64_t k = 0; k < topology->links_count; ++k)
	{
		SCENE_link_t *link = topology->links_list[k];

		char *negative = SCENE_get_negative_link(link);

		struct port_uid *port_uid = getdata_from_hash_table(table_negative, negative);
		assert(port_uid);
		link->nuid = port_uid->port_uid;
		//SCENE_print_link(link);
	}
	free_hash_table(table_negative);
	for(uint64_t x = 0; x < count; ++x)
	{
	 	//printf("[%ld] = %d\n", x, devices[x]);
	}
	topology->links_symbolized = true;

}

void SCENE_print_topology(SCENE_topology_t *topology)
{

	if(topology->links_finished)
	{
		for(uint64_t i = 0; i < topology->links_count; ++i)
		{
			SCENE_print_link(topology->links_list[i]);
		}
	}
	if(topology->links_symbolized)
	{
	}
}



/*

static void CFG_init_stage2_context(CFG_context_t *ctx)

{
	const uint64_t wctx_count = ctx->wireconfigs->count;
	const uint64_t wctx_highid = ctx->wireconfigs->highestid;
	const uint64_t count = CFG_get_count_link_buf(ctx->channelbuf);
	assert(wctx_highid < 100 && wctx_highid > 0);

	//map to each wire id
	CFG_edge_t *map[wctx_highid];
	memset(map, 0, sizeof(CFG_edge_t *) * wctx_count);


	for(uint64_t k = 0; k < wctx_count; ++k)
	{
		CFG_edge_t *wirecfg = &ctx->wireconfigs->cfgs[k];
		//printf("id: %ld\n", wirecfg->id);
		map[wirecfg->id] = wirecfg;
	}

	for(uint64_t i = 0; i < count; ++i)
	{
		CFG_link_t *cfg =  CFG_get_index_link_buf(ctx->channelbuf, i);
		assert(cfg->devcfg != NULL);
		//printf("%d\n", cfg->wireid);
		assert(map[cfg->wireid] != NULL && "node had attached wire that waas not initalized");
		cfg->edge = map[cfg->wireid];
		assert(cfg->edge->id == cfg->wireid);
		CFG_edge_id_t cid = cfg->chnlid;
		CFG_append_edge(cfg->edge, cid);

	}


	ctx->table = CFG_init_map(ctx);
}

static void CFG_init_stage3_context(CFG_context_t *ctx)
{
	const uint64_t devcount = ctx->deviceconfigs->count;
	const uint64_t wirecount = ctx->wireconfigs->count;
	CFG_node_id_t dtags[devcount];

	for(uint64_t i = 0; i < devcount; ++i)
	{
		CFG_mapelm_t *elm = CFG_get_from_index_map(ctx->table, i);
	 	dtags[i] = elm->tag;
		//printf("dtags[%ld]=%ld\n",i, dtags[i]);
	}

	//generate routetable information
	//for each device init it's route table with the dtags

	for(uint64_t k = 0; k < devcount; ++k)
	{
		//the current device
		CFG_node_t *active = &ctx->deviceconfigs->cfgs[k];
		//init the routetable
		CFG_init_routetable_node(active, devcount, dtags, wirecount);


	}
}
*/

void SCENE_build_topology(SCENE_topology_t *topology)
{

	uint64_t links_count = topology->links_count;
	uint64_t ports_count = topology->ports_count;


	bool *links_seen = calloc(links_count * ports_count, sizeof(bool));
	bool *ports_seen = calloc(links_count * ports_count, sizeof(bool));




	for(uint64_t i = 0; i < ports_count; ++i)
	{
		SCENE_port_t *port = topology->ports_list[i];
		for(uint64_t l = 0; l < port->links_count; ++l)
		{
			uint64_t index = port->links[l];

			SCENE_link_t *link = topology->links_list[index];
			//SCENE_print_link(link);	
			links_seen[(i * links_count) + index] = true ;
			ports_seen[(index * ports_count) + i] = true;			
		}

	}

	
	for(uint64_t x = 0; x < ports_count; ++x)
	{
		for(uint64_t y = 0; y < links_count; ++y)
		{
			printf("%d ", (int)links_seen[(x * links_count) + y]);
		}
		printf("\n");
	}
	printf("\n-------\n");
	for(uint64_t x = 0; x < links_count; ++x)
	{
		for(uint64_t y = 0; y < ports_count; ++y)
		{
			printf("%d ", (int)ports_seen[(x * ports_count) + y]);
		}
		printf("\n");
	}
	//
	//[a->c]
	//[a->b]



	/*uint64_t devcfg_count = ctx->deviceconfigs->count;
	uint64_t wirecfg_count = ctx->wireconfigs->count;


	bool dev_wire_seen[devcfg_count][wirecfg_count];
	bool wire_dev_seen[wirecfg_count][devcfg_count];


	memset(dev_wire_seen, 0, sizeof(dev_wire_seen));
	memset(wire_dev_seen, 0, sizeof(wire_dev_seen));

	uint64_t chnlbuf_count = CFG_get_count_link_buf(ctx->channelbuf);


	for(uint64_t i = 0; i < chnlbuf_count; ++i)
	{
		CFG_link_t *chnlcfg = CFG_get_index_link_buf(ctx->channelbuf, i);

		uint64_t device_index = chnlcfg->devcfg - ctx->deviceconfigs->cfgs;
		uint64_t wire_index = chnlcfg->edge - ctx->wireconfigs->cfgs;

		dev_wire_seen[device_index][wire_index] = true;
		wire_dev_seen[wire_index][device_index] = true;
	}





	uint64_t *distances = calloc(devcfg_count * devcfg_count, sizeof(uint64_t));
	uint64_t *wire_dist = calloc(wirecfg_count, sizeof(uint64_t));

	int32_t *first_wire_to_dev = calloc(devcfg_count, sizeof(int32_t));
	int32_t *first_wire_to_wire = calloc(wirecfg_count, sizeof(int32_t));
	//FOR EACH SOURCE
	for(uint64_t s = 0; s < devcfg_count; ++s)
	{
		uint64_t *dev_dist = distances + (s * devcfg_count);
		memset(dev_dist,  0xfffffff, sizeof(uint64_t) * devcfg_count);
		memset(wire_dist, 0xffffffff, sizeof(uint64_t) * wirecfg_count);


		memset(first_wire_to_dev, 0xffff, sizeof(int32_t) * devcfg_count);
		memset(first_wire_to_wire, 0xffff, sizeof(int32_t) * wirecfg_count);

		dev_dist[s] = 0;
		//SET ANYTHING THAT'S IMMEDATELY SEEABLE AND MAKE IT WRITEABLE
		for(uint64_t w = 0; w < wirecfg_count; ++w)
		{
			if(dev_wire_seen[s][w])
			{
				CFG_edge_t *wire = &ctx->wireconfigs->cfgs[w];
				wire_dist[w] = wire->latency;
				first_wire_to_wire[w] = w;
			}
		}

		bool changed = true;
		while(changed)
		{

			changed = false;

			for(uint64_t d1 = 0; d1 < devcfg_count; ++d1)
			{
				if(dev_dist[d1] > 100000) continue;
				for(uint64_t w1 = 0; w1 < wirecfg_count; ++w1)
				{
					if(!dev_wire_seen[d1][w1]) continue;
					CFG_edge_t *wire = &ctx->wireconfigs->cfgs[w1];
					uint64_t cost = dev_dist[d1] + wire->latency;
					if(cost < wire_dist[w1])
					{
						wire_dist[w1] = cost;

						uint64_t tmp = (d1 == s) ? w1 : first_wire_to_dev[d1];
            			first_wire_to_wire[w1] = tmp;
              			changed = true;
					}
				}
			}
			//printf("\n");

			//w2
			for(uint64_t w2 = 0; w2 < wirecfg_count; ++w2)
			{

				if(wire_dist[w2] > 100000) continue;
				for(uint64_t d2 = 0; d2 < devcfg_count; ++d2)
				{
					//check if the wire is seen by the device
					if(!wire_dev_seen[w2][d2]) continue;
					if(wire_dist[w2] < dev_dist[d2])
					{
						dev_dist[d2] = wire_dist[w2];
            			first_wire_to_dev[d2] = first_wire_to_wire[w2];
              			changed = true;
					}
				}
			}


		}


	}

	for(uint64_t a = 0; a < devcfg_count; ++a)
	{

		CFG_node_t *src = &ctx->deviceconfigs->cfgs[a];

		for(uint64_t b = 0; b < wirecfg_count; ++b)
		{

			if(!dev_wire_seen[a][b])
			{
				continue;
			}
			//for this wire
			int32_t lowest_cost[devcfg_count];
			memset(lowest_cost, 0xff, sizeof(lowest_cost));

			//lowest cost distance on the wire

			//check each device if it's distance is less than the lowest_cost
			for(uint64_t c = 0; c < devcfg_count; ++c)
			{
				if(!wire_dev_seen[b][c]) continue;
				if(c == a) continue;
				uint64_t *row = distances + (c * devcfg_count);

				for(uint64_t d = 0; d < devcfg_count; ++d)
				{
					if(d == a) continue;
					if( row[d] < lowest_cost[d])
						 lowest_cost[d] = row[d];
				}


			}

			for(uint64_t e = 0; e < devcfg_count; ++e)
			{
				if(e == a) continue;
				if(lowest_cost[e] > 100000) continue;
				CFG_edge_t *wire = &ctx->wireconfigs->cfgs[b];

				CFG_node_route_t route =
				{
        			.latency = lowest_cost[e] + wire->latency,
        			.wire    = wire->id,
					.index = b
    			};
	    		CFG_append_route_node(src, e, route);
			}

		}



	}

	free(distances);
	free(wire_dist);
	free(first_wire_to_dev);
	free(first_wire_to_wire);

	*/

	free(ports_seen);
	free(links_seen);
}
