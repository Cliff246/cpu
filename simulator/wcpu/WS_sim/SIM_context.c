#include "SIM_context.h"
#include "CFG_manifest.h"

#include "SIM_chnlcfg.h"
#include "SIM_context.h"
#include "SIM_device.h"
#include "SIM_devcfg.h"

#include "commons.h"
#include "SIM_wirecfg.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


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
		SIM_wireconfig_print(&cfglist[i]);
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


SIM_context_t *SIM_init_context(CFG_manifest_t *manifest)
{
	SIM_context_t *ctx = calloc(1, sizeof(SIM_context_t));
	assert(ctx);

	ctx->channelbuf = SIM_init_chnlcfg_buf(10);
	assert(ctx->channelbuf);

	ctx->deviceconfigs = SIM_init_devcfg_ctx(manifest, ctx);

	ctx->wireconfigs = SIM_init_wirecfg_ctx(manifest, ctx);
	const uint64_t wctx_count = ctx->wireconfigs->count;
	const uint64_t wctx_highid = ctx->wireconfigs->highestid;
	SIM_qsort_wirecfg_ctx(ctx->wireconfigs, 0, wctx_count - 1);
	CFG_free_manifest(manifest);
	const uint64_t count = SIM_get_count_chnlcfg_buf(ctx->channelbuf);
	assert(wctx_highid < 100 && wctx_highid > 0);
	SIM_wirecfg_t *map[wctx_count];

	for(uint64_t k = 0; k < wctx_count; ++k)
	{
		SIM_wirecfg_t *wirecfg = &ctx->wireconfigs->cfgs[k];
		printf("id: %ld\n", wirecfg->id);
		map[wirecfg->id] = wirecfg;
	}

	for(uint64_t i = 0; i < count; ++i)
	{
		SIM_chnlcfg_t *cfg =  SIM_get_index_chnlcfg(ctx->channelbuf, i);
		assert(cfg->devcfg != NULL);
		cfg->wirecfg = map[cfg->wireid];
		assert(cfg->wirecfg->id == cfg->wireid);
		cfg->wirecfg->channels++;

		SIM_print_chnlcfg(cfg);
	}

	ctx->table = SIM_init_vistable(ctx);

	return ctx;
}

static void SIM_free_devcfg_ctx(SIM_devcfg_ctx_t *cfg)
{
	for(int i = 0; i < cfg->count; ++i)
	{
		SIM_free_devcfg(&cfg->cfgs[i]);
	}
	free(cfg->cfgs);
	free(cfg);
}

static void SIM_free_wirecfg_ctx(SIM_wirecfg_ctx_t *cfg)
{

}

void SIM_free_context(SIM_context_t *context)
{
	SIM_free_devcfg_ctx(context->deviceconfigs);
	SIM_free_wirecfg_ctx(context->wireconfigs);
}

/*

SIM_context_t *SIM_init_context(CFG_manifest_t *manifest)
{
	SIM_context_t *ctx = calloc(1, sizeof(SIM_context_t));
	assert(ctx);

	ctx->devicelist = SIM_context_init_devicelist(manifest);
	const uint64_t size = manifest->settings_size;
	assert(size > 0);
	//printf("size: %d\n", size);
	SIM_wireconfig_t *wireconfigs = calloc(size, sizeof(SIM_wireconfig_t));
	assert(wireconfigs);

	for(uint64_t i = 0; i < size; ++i)
	{
		CFG_setting_t *setting = manifest->settings[i];

		SIM_wireconfig_t connector =
		{
			.id = setting->id,
			.latency = setting->latency,
		};

		wireconfigs[i] = connector;
		SIM_wireconfig_print(&connector);
	}
	ctx->wireconfigs = wireconfigs;
	ctx->wireconfigs_size = size;
	return ctx;
}

/*

	SIM_context_t *context = graph->context;
	const uint64_t size = SIM_get_devicelist_size(context->devicelist);
	for(uint64_t i = 0; i < context->wireconfigs_size; ++i)
	{
		//printf("%d\n", i);
		SIM_wireconfig_t *wireconfig = &context->wireconfigs[i];
		SIM_channel_t *channels[size];
		uint64_t j = 0;

		for(uint64_t k = 0; k < size; ++k)
		{
			SIM_device_t *device = SIM_get_device_devicelist(context->devicelist, k);
			SIM_channel_t *channel = SIM_device_get_channel_by_wireid(device, wireconfig->id);
			if(channel == NULL)
				continue;
			channels[j++] = channel;
		}
		//printf("J:%d\n", j);
		SIM_wire_t *wire = SIM_init_wire(channels, j, *wireconfig);
		SIM_print_wire(wire);
	}


*/