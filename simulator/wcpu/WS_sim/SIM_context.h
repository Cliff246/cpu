#ifndef __WS_SIM_CONTEXT_HEADER__
#define __WS_SIM_CONTEXT_HEADER__

#include "SIM_channel.h"
#include "SIM_chnlcfg.h"
#include "SIM_device.h"
#include "SIM_devcfg.h"
#include "CFG_manifest.h"
#include "SIM_wirecfg.h"

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>


typedef struct WS_SIM_wirecfg_context
{
	uint64_t count;
	uint64_t highestid;
	SIM_wirecfg_t *cfgs;

}SIM_wirecfg_ctx_t;




typedef struct WS_SIM_devcfg_context
{
	uint64_t count;

	SIM_devcfg_t *cfgs;

}SIM_devcfg_ctx_t;








typedef struct WS_SIM_context
{

	SIM_devcfg_ctx_t *deviceconfigs;
	SIM_wirecfg_ctx_t *wireconfigs;
	SIM_chnlcfg_buf_t *channelbuf;
}SIM_context_t;

static void SIM_swap_wirecfg_ctx(SIM_wirecfg_ctx_t *ctx, size_t i, size_t j);
static size_t SIM_partition_wirecfg_ctx(SIM_wirecfg_ctx_t *ctx, size_t lo, size_t hi);
static void SIM_qsort_wirecfg_ctx(SIM_wirecfg_ctx_t *ctx, size_t lo, size_t hi);
static SIM_wirecfg_t *SIM_bsearch_wirecfg_ctx(SIM_wirecfg_ctx_t *ctx, uint64_t i);

static SIM_wirecfg_ctx_t *SIM_init_wirecfg_ctx(CFG_manifest_t *manifest, SIM_context_t *context);
bool SIM_sort_wirecfg_ctx(SIM_wirecfg_ctx_t *wire);
static SIM_devcfg_ctx_t *SIM_init_devcfg_ctx(CFG_manifest_t *manifest, SIM_context_t *context);
static void SIM_free_devcfg_ctx(SIM_devcfg_ctx_t *cfg);
static void SIM_free_wirecfg_ctx(SIM_wirecfg_ctx_t *cfg);
SIM_context_t *SIM_init_context(CFG_manifest_t *manifest);
void SIM_free_context(SIM_context_t *context);

#endif