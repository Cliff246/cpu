#include "CFG_context.h"
#include "CFG_manifest.h"

#include "CFG_link.h"
#include "CFG_commons.h"
#include "CFG_node.h"
#include "CFG_edge.h"

#include "CFG_map.h"
#include "commons.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


static CFG_node_ctx_t *CFG_init_node_ctx(CFG_manifest_t *manifest, CFG_context_t *context)
{
	CFG_node_ctx_t *cfgctx = calloc(1, sizeof(CFG_node_ctx_t));
	assert(cfgctx);
	const uint64_t size = manifest->size;
	assert(size > 0);

	CFG_node_t *cfglist = calloc(size, sizeof(CFG_node_t));
	assert(cfglist);

	for(int i = 0; i < size; ++i)
	{
		CFG_entry_t *entry = manifest->entries[i];
		CFG_node_t *cfg = &cfglist[i];
		bool passed = CFG_init_node(cfg, entry, context->channelbuf);

		assert(passed);

	}
	cfgctx->cfgs = cfglist;
	cfgctx->count = size;

	return cfgctx;
}


static CFG_edge_ctx_t *CFG_init_edge_ctx(CFG_manifest_t *manifest, CFG_context_t *context)
{
	CFG_edge_ctx_t *cfgctx = calloc(1, sizeof(CFG_edge_ctx_t));
	assert(cfgctx);

	const uint64_t size = manifest->settings_size;
	assert(size > 0);
	//printf("size: %d\n", size);

	CFG_edge_t *cfglist = calloc(size, sizeof(CFG_edge_t));
	assert(cfglist);

	for(uint64_t i = 0; i < size; ++i)
	{
		CFG_setting_t *setting = manifest->settings[i];


		CFG_init_edge(&cfglist[i], setting->id, setting->latency,setting->throughput);
		cfgctx->highestid = MAX(setting->id, cfgctx->highestid);
		CFG_print_edge(&cfglist[i]);
	}

	cfgctx->count = size;
	cfgctx->cfgs = cfglist;

	return cfgctx;
}

static void CFG_swap_edge_ctx(CFG_edge_ctx_t *ctx, size_t i, size_t j)
{

	CFG_edge_t temp = ctx->cfgs[i];
	ctx->cfgs[i] = ctx->cfgs[j];
	ctx->cfgs[j] = temp;
}

static size_t CFG_partition_edge_ctx(CFG_edge_ctx_t *ctx, size_t lo, size_t hi)
{
	assert(ctx);
	size_t pivot = ctx->cfgs[hi].id;

	size_t i = lo;

	for(size_t j = lo; j <= (hi - 1); ++j)
	{
		if(ctx->cfgs[j].id < pivot)
		{
           	CFG_swap_edge_ctx(ctx, i, j);
            i++;
		}
	}

    CFG_swap_edge_ctx(ctx, i, hi);
    return i;
}

static void CFG_qsort_edge_ctx(CFG_edge_ctx_t *ctx, size_t lo, size_t hi)
{
	if(lo >= hi) return;
	size_t pivot = CFG_partition_edge_ctx(ctx, lo, hi);
	if(pivot > 0)
		CFG_qsort_edge_ctx(ctx, lo, pivot - 1);
	CFG_qsort_edge_ctx(ctx, pivot + 1, hi);
}

//takes a SIM_wireid_t
static CFG_edge_t *CFG_bsearch_edge_ctx(CFG_edge_ctx_t *ctx, uint64_t i)
{
	uint64_t lo = 0;
	uint64_t hi = ctx->count - 1;
	while(lo < hi)
	{
		uint64_t mi = lo + (hi - lo) / 2;
		if(ctx->cfgs[mi].id == i)
			return &ctx->cfgs[mi];
		else if(ctx->cfgs[mi].id < i)
		{
			lo = mi + 1;
		}
		else
			hi = mi - 1;
	}
	return NULL;
}

static void CFG_init_stage1_context(CFG_context_t *ctx, CFG_manifest_t *manifest)
{



	ctx->channelbuf = CFG_init_link_buf(10);
	assert(ctx->channelbuf);

	ctx->deviceconfigs = CFG_init_node_ctx(manifest, ctx);
	//set up wires
	ctx->wireconfigs = CFG_init_edge_ctx(manifest, ctx);


	const uint64_t wctx_count = ctx->wireconfigs->count;


	//sort the wires based on id
	CFG_qsort_edge_ctx(ctx->wireconfigs, 0, wctx_count - 1);

	//free the manifest
	CFG_free_manifest(manifest);


}

static void CFG_init_stage2_context(CFG_context_t *ctx)

{
	const uint64_t wctx_count = ctx->wireconfigs->count;
	const uint64_t wctx_highid = ctx->wireconfigs->highestid;
	const uint64_t count = CFG_get_count_link_buf(ctx->channelbuf);
	assert(wctx_highid < 100 && wctx_highid > 0);

	//map to each wire id
	CFG_edge_t *map[wctx_count];
	memset(map, 0, sizeof(CFG_edge_t *) * wctx_count);


	for(uint64_t k = 0; k < wctx_count; ++k)
	{
		CFG_edge_t *wirecfg = &ctx->wireconfigs->cfgs[k];
	//	printf("id: %ld\n", wirecfg->id);
		map[wirecfg->id] = wirecfg;
	}

	for(uint64_t i = 0; i < count; ++i)
	{
		CFG_link_t *cfg =  CFG_get_index_link_buf(ctx->channelbuf, i);
		assert(cfg->devcfg != NULL);
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
		CFG_init_routetable_node(active, devcount, dtags);


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


	printf("wireid: ");
	for(uint64_t head = 0; head < wirecfg_count; ++head)
	{
		printf("%ld ", head);
	}
	printf("\n");
	for(int a = 0; a < devcfg_count; ++a)
	{
		printf("%c:\t", a + 'a');
		for(uint64_t b = 0; b <wirecfg_count; ++b)
		{
			printf("%d ", dev_wire_seen[a][b]);
		}
		printf("\n");
	}

	printf("\n");


	printf("devid:  ");
	for(uint64_t head1 = 0; head1 < devcfg_count; ++head1)
	{
		printf("%c ", head1 + 'a');
	}
	printf("\n");
	for(int a = 0; a < wirecfg_count; ++a)
	{
		printf("%d:\t", a);
		for(uint64_t b = 0; b <devcfg_count; ++b)
		{
			printf("%d ", wire_dev_seen[a][b]);
		}
		printf("\n");
	}
	printf("\n");

	//FOR EACH SOURCE
	for(uint64_t s = 0; s < devcfg_count; ++s)
	{

		uint64_t dev_dist[devcfg_count];
		uint64_t wire_dist[wirecfg_count];
		memset(dev_dist, 0xffffffff, sizeof(dev_dist));
		memset(wire_dist, 0xffffffff, sizeof(wire_dist));

		//this needs to be 2d
		int32_t first_wire_to_dev[devcfg_count];
		//
		int32_t first_wire_to_wire[wirecfg_count];

		memset(first_wire_to_dev, 0xffff, sizeof(first_wire_to_dev));
		memset(first_wire_to_wire, 0xffff, sizeof(first_wire_to_wire));

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
			for(uint64_t w2 = 0; w2 < wirecfg_count; ++w2)
			{
				if(wire_dist[w2] > 100000) continue;
				for(uint64_t d2 = 0; d2 < devcfg_count; ++d2)
				{
					if(!wire_dev_seen[w2][d2]) continue;
					if(wire_dist[w2] < dev_dist[d2])
					{
						dev_dist[d2] = wire_dist[w2];
            			first_wire_to_dev[d2] = first_wire_to_wire[w2];
              			changed = true;
					}
				}
			}

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
            			first_wire_to_wire[w1] = (d1 == s) ? w1 : first_wire_to_dev[d1];
              			changed = true;
					}
				}
			}
		}

		CFG_node_t *src = &ctx->deviceconfigs->cfgs[s];
		printf("first wire to dev\n");
		for(uint64_t pfwtd = 0; pfwtd < devcfg_count; ++pfwtd)
		{
			printf("%d ", first_wire_to_dev[pfwtd]);
		}
		printf("\n");

		printf("first wire to wire\n");
		for(uint64_t pfwtw = 0; pfwtw < wirecfg_count; ++pfwtw)
		{
			printf("%d ", first_wire_to_wire[pfwtw]);
		}
		printf("\n");

		for (uint64_t d = 0; d < devcfg_count; ++d)
		{
    		if (d == s) continue;
    		if (first_wire_to_dev[d] < 0) continue; // unreachable

    		CFG_node_route_t route =
			{
        		.latency = dev_dist[d],
        		.wire    = (CFG_edge_id_t)first_wire_to_dev[d]
    		};

    		CFG_append_route_node(src, d, route);
		}
		printf("\n");
	}





}
CFG_context_t *CFG_init_context(CFG_manifest_t *manifest)
{
	CFG_context_t *ctx = calloc(1, sizeof(CFG_context_t));
	assert(ctx);
	CFG_init_stage1_context(ctx, manifest);
	CFG_init_stage2_context(ctx);
	CFG_init_stage3_context(ctx);
	CFG_init_stage4_context(ctx);


	return ctx;
}


static void CFG_free_node_ctx(CFG_node_ctx_t *cfg)
{
	for(uint64_t i = 0; i < cfg->count; ++i)
	{
		CFG_free_node(&cfg->cfgs[i]);
	}
	free(cfg->cfgs);
	free(cfg);
}
static void CFG_free_edge_ctx(CFG_edge_ctx_t *cfg)
{
	for(uint64_t i = 0; i < cfg->count; ++i)
	{
		CFG_free_edge(&cfg->cfgs[i]);

	}
	free(cfg->cfgs);
	free(cfg);
}
void CFG_free_context(CFG_context_t *context)
{
	CFG_free_node_ctx(context->deviceconfigs);
	CFG_free_edge_ctx(context->wireconfigs);
	CFG_free_map(context->table);
	CFG_free_link_buf(context->channelbuf);
	free(context);
}

