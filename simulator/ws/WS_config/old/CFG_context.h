#ifndef _CFG_CONTEXT_HEADER__
#define _CFG_CONTEXT_HEADER__

#include "MANFST_manifest.h"
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>





typedef struct CFG_context
{


}CFG_context_t;
CFG_context_t *CFG_init_context(MANFST_manifest_t *manifest);
/*

static void CFG_swap_edge_ctx(CFG_edge_ctx_t *ctx, size_t i, size_t j);
static size_t CFG_partition_edge_ctx(CFG_edge_ctx_t *ctx, size_t lo, size_t hi);

static void CFG_qsort_edge_ctx(CFG_edge_ctx_t *ctx, size_t lo, size_t hi);
static CFG_edge_t *CFG_bsearch_edge_ctx(CFG_edge_ctx_t *ctx, uint64_t i);

static CFG_edge_ctx_t *CFG_init_edge_ctx(MANFST_manifest_t *manifest, CFG_context_t *context);
static CFG_node_ctx_t *CFG_init_node_ctx(MANFST_manifest_t *manifest, CFG_context_t *context);
static void CFG_free_node_ctx(CFG_node_ctx_t *cfg);
static void CFG_free_edge_ctx(CFG_edge_ctx_t *cfg);

static void CFG_init_stage1_context(CFG_context_t *ctx, MANFST_manifest_t *manifest);
static void CFG_init_stage2_context(CFG_context_t *ctx);
static void CFG_init_stage3_context(CFG_context_t *ctx);
static void CFG_init_stage4_context(CFG_context_t *ctx);

//init the context
CFG_context_t *CFG_init_context(MANFST_manifest_t *manifest);
void CFG_free_context(CFG_context_t *context);

*/

#endif