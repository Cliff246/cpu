#include "SIM_context.h"
#include "CFG_manifest.h"

#include "SIM_chnlcfg.h"
#include "SIM_commons.h"
#include "SIM_context.h"
#include "SIM_device.h"
#include "SIM_devcfg.h"

#include "SIM_vistable.h"
#include "commons.h"
#include "SIM_wirecfg.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


static SIM_devcfg_ctx_t *SIM_init_devcfg_ctx(CFG_manifest_t *manifest, SIM_context_t *context)
{
	SIM_devcfg_ctx_t *cfgctx = calloc(1, sizeof(SIM_devcfg_ctx_t));
	assert(cfgctx);
	const uint64_t size = manifest->size;
	assert(size > 0);

	SIM_devcfg_t *cfglist = calloc(size, sizeof(SIM_devcfg_t));
	assert(cfglist);

	for(int i = 0; i < size; ++i)
	{
		CFG_entry_t *entry = manifest->entries[i];
		SIM_devcfg_t *cfg = &cfglist[i];
		bool passed = SIM_init_devcfg(cfg, entry, context->channelbuf);

		assert(passed);

	}
	cfgctx->cfgs = cfglist;
	cfgctx->count = size;

	return cfgctx;
}



static SIM_wirecfg_ctx_t *SIM_init_wirecfg_ctx(CFG_manifest_t *manifest, SIM_context_t *context)
{
	SIM_wirecfg_ctx_t *cfgctx = calloc(1, sizeof(SIM_wirecfg_ctx_t));
	assert(cfgctx);

	const uint64_t size = manifest->settings_size;
	assert(size > 0);
	//printf("size: %d\n", size);

	SIM_wirecfg_t *cfglist = calloc(size, sizeof(SIM_wirecfg_t));
	assert(cfglist);

	for(uint64_t i = 0; i < size; ++i)
	{
		CFG_setting_t *setting = manifest->settings[i];


		SIM_init_wirecfg(&cfglist[i], setting->id, setting->latency,setting->throughput);
		cfgctx->highestid = MAX(setting->id, cfgctx->highestid);
		SIM_print_wirecfg(&cfglist[i]);
	}

	cfgctx->count = size;
	cfgctx->cfgs = cfglist;

	return cfgctx;
}

static void SIM_swap_wirecfg_ctx(SIM_wirecfg_ctx_t *ctx, size_t i, size_t j)
{

	SIM_wirecfg_t temp = ctx->cfgs[i];
	ctx->cfgs[i] = ctx->cfgs[j];
	ctx->cfgs[j] = temp;
}


static size_t SIM_partition_wirecfg_ctx(SIM_wirecfg_ctx_t *ctx, size_t lo, size_t hi)
{
	assert(ctx);
	size_t pivot = ctx->cfgs[hi].id;

	size_t i = lo;

	for(size_t j = lo; j <= (hi - 1); ++j)
	{
		if(ctx->cfgs[j].id < pivot)
		{
           	SIM_swap_wirecfg_ctx(ctx, i, j);
            i++;
		}
	}

    SIM_swap_wirecfg_ctx(ctx, i, hi);
    return i;
}

static void SIM_qsort_wirecfg_ctx(SIM_wirecfg_ctx_t *ctx, size_t lo, size_t hi)
{
	if(lo >= hi) return;
	size_t pivot = SIM_partition_wirecfg_ctx(ctx, lo, hi);
	if(pivot > 0)
		SIM_qsort_wirecfg_ctx(ctx, lo, pivot - 1);
	SIM_qsort_wirecfg_ctx(ctx, pivot + 1, hi);
}

//takes a SIM_wireid_t
static SIM_wirecfg_t *SIM_bsearch_wirecfg_ctx(SIM_wirecfg_ctx_t *ctx, uint64_t i)
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

static void SIM_init_stage1_context(SIM_context_t *ctx, CFG_manifest_t *manifest)
{



	ctx->channelbuf = SIM_init_chnlcfg_buf(10);
	assert(ctx->channelbuf);

	ctx->deviceconfigs = SIM_init_devcfg_ctx(manifest, ctx);
	//set up wires
	ctx->wireconfigs = SIM_init_wirecfg_ctx(manifest, ctx);


	const uint64_t wctx_count = ctx->wireconfigs->count;


	//sort the wires based on id
	SIM_qsort_wirecfg_ctx(ctx->wireconfigs, 0, wctx_count - 1);

	//free the manifest
	CFG_free_manifest(manifest);


}

static void SIM_init_stage2_context(SIM_context_t *ctx)
{
	const uint64_t wctx_count = ctx->wireconfigs->count;
	const uint64_t wctx_highid = ctx->wireconfigs->highestid;
	const uint64_t count = SIM_get_count_chnlcfg_buf(ctx->channelbuf);
	assert(wctx_highid < 100 && wctx_highid > 0);

	//map to each wire id
	SIM_wirecfg_t *map[wctx_count];
	memset(map, 0, sizeof(SIM_wirecfg_t *) * wctx_count);


	for(uint64_t k = 0; k < wctx_count; ++k)
	{
		SIM_wirecfg_t *wirecfg = &ctx->wireconfigs->cfgs[k];
	//	printf("id: %ld\n", wirecfg->id);
		map[wirecfg->id] = wirecfg;
	}

	for(uint64_t i = 0; i < count; ++i)
	{
		SIM_chnlcfg_t *cfg =  SIM_get_index_chnlcfg(ctx->channelbuf, i);
		assert(cfg->devcfg != NULL);
		cfg->wirecfg = map[cfg->wireid];
		assert(cfg->wirecfg->id == cfg->wireid);
		SIM_chnlid_t cid = cfg->chnlid;
		SIM_append_wirecfg(cfg->wirecfg, cid);

	}


	ctx->table = SIM_init_vistable(ctx);
}

static void SIM_init_stage3_context(SIM_context_t *ctx)
{
	const uint64_t devcount = ctx->deviceconfigs->count;
	const uint64_t wirecount = ctx->wireconfigs->count;
	SIM_dtag_t dtags[devcount];

	for(uint64_t i = 0; i < devcount; ++i)
	{
		SIM_viselm_t *elm = SIM_get_from_index_vistable(ctx->table, i);
	 	dtags[i] = elm->tag;
		//printf("dtags[%ld]=%ld\n",i, dtags[i]);
	}

	//generate routetable information
	//for each device init it's route table with the dtags

	for(uint64_t k = 0; k < devcount; ++k)
	{
		//the current device
		SIM_devcfg_t *active = &ctx->deviceconfigs->cfgs[k];
		//init the routetable
		SIM_init_routetable_devcfg(active, devcount, dtags);


	}
}


static void SIM_init_stage4_context(SIM_context_t *ctx)
{
	uint64_t devcfg_count = ctx->deviceconfigs->count;
	uint64_t wirecfg_count = ctx->wireconfigs->count;


	bool dev_wire_seen[devcfg_count][wirecfg_count];
	bool wire_dev_seen[wirecfg_count][devcfg_count];


	memset(dev_wire_seen, 0, sizeof(dev_wire_seen));
	memset(wire_dev_seen, 0, sizeof(wire_dev_seen));

	uint64_t chnlbuf_count = SIM_get_count_chnlcfg_buf(ctx->channelbuf);


	for(uint64_t i = 0; i < chnlbuf_count; ++i)
	{
		SIM_chnlcfg_t *chnlcfg = SIM_get_index_chnlcfg(ctx->channelbuf, i);

		uint64_t device_index = chnlcfg->devcfg - ctx->deviceconfigs->cfgs;
		uint64_t wire_index = chnlcfg->wirecfg - ctx->wireconfigs->cfgs;

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
		printf("%d:\t", a);
		for(uint64_t b = 0; b <wirecfg_count; ++b)
		{
			printf("%d ", dev_wire_seen[a][b]);
		}
		printf("\n");
	}


	for(uint64_t s = 0; s < devcfg_count; ++s)
	{

		uint64_t dev_dist[devcfg_count];
		uint64_t wire_dist[wirecfg_count];
		memset(dev_dist, 0xffffffff, sizeof(dev_dist));
		memset(wire_dist, 0xffffffff, sizeof(wire_dist));


		int32_t first_wire_to_dev[devcfg_count];
		int32_t first_wire_to_wire[wirecfg_count];

		memset(first_wire_to_dev, 0xffff, sizeof(first_wire_to_dev));
		memset(first_wire_to_wire, 0xffff, sizeof(first_wire_to_wire));

		dev_dist[s] = 0;

		for(uint64_t w = 0; w < wirecfg_count; ++w)
		{
			if(dev_wire_seen[s][w])
			{
				SIM_wirecfg_t *wire = &ctx->wireconfigs->cfgs[w];
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
					SIM_wirecfg_t *wire = &ctx->wireconfigs->cfgs[w1];
					uint64_t cost = dev_dist[d1] + wire->latency;
					if(cost < wire_dist[w1])
					{
						wire_dist[w1] = cost;
            			first_wire_to_wire[w1] = (d1 == s) ? w1 : first_wire_to_dev[d1];
              			changed = true;
					}
				}
			}
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
		}
		SIM_devcfg_t *src = &ctx->deviceconfigs->cfgs[s];

		for (uint64_t d = 0; d < devcfg_count; ++d)
		{
    		if (d == s) continue;
    		if (first_wire_to_dev[d] < 0) continue; // unreachable

    		SIM_devcfg_route_t route =
			{
        		.latency = dev_dist[d],
        		.wire    = (SIM_wireid_t)first_wire_to_dev[d]
    		};

    		SIM_append_route_devcfg(src, d, route);
		}

	}





}

SIM_context_t *SIM_init_context(CFG_manifest_t *manifest)
{
	SIM_context_t *ctx = calloc(1, sizeof(SIM_context_t));
	assert(ctx);
	SIM_init_stage1_context(ctx, manifest);
	SIM_init_stage2_context(ctx);
	SIM_init_stage3_context(ctx);
	SIM_init_stage4_context(ctx);


	return ctx;
}

static void SIM_free_devcfg_ctx(SIM_devcfg_ctx_t *cfg)
{
	for(uint64_t i = 0; i < cfg->count; ++i)
	{
		SIM_free_devcfg(&cfg->cfgs[i]);
	}
	free(cfg->cfgs);
	free(cfg);
}

static void SIM_free_wirecfg_ctx(SIM_wirecfg_ctx_t *cfg)
{
	for(uint64_t i = 0; i < cfg->count; ++i)
	{
		SIM_free_wirecfg(&cfg->cfgs[i]);

	}
	free(cfg->cfgs);
	free(cfg);
}

void SIM_free_context(SIM_context_t *context)
{
	SIM_free_devcfg_ctx(context->deviceconfigs);
	SIM_free_wirecfg_ctx(context->wireconfigs);
	SIM_free_vistable(context->table);
	SIM_free_chnlcfg_buf(context->channelbuf);
	free(context);
}

