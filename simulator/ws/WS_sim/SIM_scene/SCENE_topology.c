#include "SCENE_topology.h"
#include <stdlib.h>


SCENE_topology_t *SCENE_init_topology(void)
{
	SCENE_topology_t *topology = calloc(1, sizeof(SCENE_topology_t));

	return topology;

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

static void CFG_init_stage4_context(CFG_context_t *ctx)
{
	uint64_t devcfg_count = ctx->deviceconfigs->count;
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


}
CFG_context_t *CFG_init_context(MANFST_manifest_t *manifest)
{
	CFG_context_t *ctx = calloc(1, sizeof(CFG_context_t));
	assert(ctx);
	CFG_init_stage1_context(ctx, manifest);
	CFG_init_stage2_context(ctx);
	CFG_init_stage3_context(ctx);
	CFG_init_stage4_context(ctx);
	for(uint64_t k = 0; k < ctx->deviceconfigs->count; ++k)
	{
		CFG_print_node(&ctx->deviceconfigs->cfgs[k]);
	}


	return ctx;
}

*/