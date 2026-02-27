#include "CFG_context.h"
#include "MANFST_manifest.h"

#include "CFG_commons.h"

#include "TAG_map.h"

#include "commons.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define INDEX2(x,y,len) ((len * y) + x)


CFG_context_t *CFG_init_context(MANFST_manifest_t *manifest)
{
	assert(0 );

	CFG_context_t *ctx = calloc(1, sizeof(CFG_context_t));
	assert(ctx);
	//CFG_init_stage1_context(ctx, manifest);
	//CFG_init_stage2_context(ctx);
	//CFG_init_stage3_context(ctx);
	//CFG_init_stage4_context(ctx);
	//for(uint64_t k = 0; k < ctx->deviceconfigs->count; ++k)
	//{
	//	CFG_print_node(&ctx->deviceconfigs->cfgs[k]);
	//}


	return ctx;
}

/*

static CFG_node_ctx_t *CFG_init_node_ctx(MANFST_manifest_t *manifest, CFG_context_t *context)
{
	CFG_node_ctx_t *cfgctx = calloc(1, sizeof(CFG_node_ctx_t));
	assert(cfgctx);
	const uint64_t size = manifest->scopes_size;// manifest->entries_size;
	assert(size > 0);

	CFG_node_t *cfglist = calloc(size, sizeof(CFG_node_t));
	assert(cfglist);
	printf("try to get\n");
	TAG_argptr_t get_key =	TAG_get_fn(TAG_MAP, TAG_FN_MAP_GET_KEY_STRING);






	printf("do this\n");
	for(int i = 0; i < size; ++i)
	{
		//TODO
		//MANFST_entry_t *entry = manifest->entries[i];
		CFG_node_t *cfg = &cfglist[i];
		TAG_tag_t *tag = manifest->scopes[i].scope;
		TAG_print(tag);

		//bool passed = CFG_init_node(cfg, entry, context->channelbuf);

		//assert(passed);
		//
	;
		//bool passed = CFG_init_node(cfg, entry, context->channelbuf);
		bool passed = CFG_init_node(cfg, NULL, context->channelbuf);

	}
	cfgctx->cfgs = cfglist;
	cfgctx->count = size;

	return cfgctx;
}


static CFG_edge_ctx_t *CFG_init_edge_ctx(MANFST_manifest_t *manifest, CFG_context_t *context)
{
	CFG_edge_ctx_t *cfgctx = calloc(1, sizeof(CFG_edge_ctx_t));
	assert(cfgctx);

	const uint64_t size = 1;//manifest->settings_size;
	assert(size > 0);
	//printf("size: %d\n", size);

	CFG_edge_t *cfglist = calloc(size, sizeof(CFG_edge_t));
	assert(cfglist);

	for(uint64_t i = 0; i < size; ++i)
	{
		//MANFST_setting_t *setting = manifest->settings[i];


		//CFG_init_edge(&cfglist[i], setting->id, setting->latency,setting->throughput);
		//cfgctx->highestid = MAX(setting->id, cfgctx->highestid);
		//CFG_print_edge(&cfglist[i]);
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

//TODO make this globally safe
CFG_node_id_t CFG_generate_tag(CFG_node_t *node)
{
	CFG_node_id_t id = ((uint64_t)rand() << 32) + rand();
	node->pretag = id;
	node->has_pretag = true;
	return id;
}


*/