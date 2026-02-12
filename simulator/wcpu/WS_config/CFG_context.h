#ifndef _CFG_CONTEXT_HEADER__
#define _CFG_CONTEXT_HEADER__

#include "SIM_channel.h"
#include "CFG_link.h"
#include "CFG_node.h"
#include "CFG_manifest.h"
#include "CFG_edge.h"
#include "CFG_map.h"
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>


typedef struct CFG_edge_context
{
	uint64_t count;
	uint64_t highestid;
	CFG_edge_t *cfgs;

}CFG_edge_ctx_t;

typedef struct CFG_node_context
{
	uint64_t count;

	CFG_node_t *cfgs;

}CFG_node_ctx_t;


typedef struct CFG_context
{

	CFG_node_ctx_t *deviceconfigs;
	CFG_edge_ctx_t *wireconfigs;
	CFG_link_buf_t *channelbuf;
	CFG_map_t *table;

}CFG_context_t;

static void CFG_swap_edge_ctx(CFG_edge_ctx_t *ctx, size_t i, size_t j);
static size_t CFG_partition_edge_ctx(CFG_edge_ctx_t *ctx, size_t lo, size_t hi);

static void CFG_qsort_edge_ctx(CFG_edge_ctx_t *ctx, size_t lo, size_t hi);
static CFG_edge_t *CFG_bsearch_edge_ctx(CFG_edge_ctx_t *ctx, uint64_t i);

static CFG_edge_ctx_t *CFG_init_edge_ctx(CFG_manifest_t *manifest, CFG_context_t *context);
static CFG_node_ctx_t *CFG_init_node_ctx(CFG_manifest_t *manifest, CFG_context_t *context);
static void CFG_free_node_ctx(CFG_node_ctx_t *cfg);
static void CFG_free_edge_ctx(CFG_edge_ctx_t *cfg);

static void CFG_init_stage1_context(CFG_context_t *ctx, CFG_manifest_t *manifest);
static void CFG_init_stage2_context(CFG_context_t *ctx);
static void CFG_init_stage3_context(CFG_context_t *ctx);
static void CFG_init_stage4_context(CFG_context_t *ctx);

//init the context
CFG_context_t *CFG_init_context(CFG_manifest_t *manifest);
void CFG_free_context(CFG_context_t *context);

#endif