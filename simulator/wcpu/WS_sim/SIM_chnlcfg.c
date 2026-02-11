#include "SIM_chnlcfg.h"
#include "SIM_commons.h"
#include "commons.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void SIM_print_chnlcfg(SIM_chnlcfg_t *cfg)
{
	printf("devcfg:%p wirecfg:%p, wireid: %ld, chnlid: %ld\n", cfg->devcfg, cfg->wirecfg, cfg->wireid, cfg->chnlid);
}

//args:
//takes the channel config buffer,
//a device configure ptr,
//a wire id array of size count,
//sisze of count,
//a buffer of channel ids of the same size count,
void SIM_append_chnlcfg_buf(SIM_chnlcfg_buf_t *buf, SIM_devcfg_t *devcfg, SIM_wireid_t *wid, uint64_t count, SIM_chnlid_t *rbuf)
{
	assert(count != 0);
	if(buf->alloca < buf->count + count)
	{
		const uint64_t diff = (buf->count + count) - buf->alloca;
		uint64_t alloca = buf->alloca + diff;
		buf->cfgs = realloc_safe(buf->cfgs, alloca, sizeof(SIM_chnlcfg_t));
		assert(buf->cfgs);
		buf->alloca = alloca;
	}

	for(uint64_t i = 0; i < count; ++i)
	{

		SIM_chnlid_t cid = buf->baseid++;
		rbuf[i] = cid;
		SIM_chnlcfg_t config =
		{
			.devcfg = devcfg,
			.wireid = wid[i],
			.chnlid = cid,
			.wirecfg = NULL,
		};

		buf->cfgs[buf->count++] = config;
	}
	return;
}

SIM_chnlcfg_buf_t *SIM_init_chnlcfg_buf(uint64_t alloca)
{
	if(alloca == 0)
		alloca = 10;
	SIM_chnlcfg_t *cfgs = calloc(alloca, sizeof(SIM_chnlcfg_t));
	assert(cfgs && "calloc cfgs");
	SIM_chnlcfg_buf_t *chnlcfgbuf = calloc(1, sizeof(SIM_chnlcfg_buf_t));
	assert(chnlcfgbuf);

	chnlcfgbuf->count = 0;

	chnlcfgbuf->alloca = alloca;
	chnlcfgbuf->cfgs = cfgs;
	return chnlcfgbuf;

}

uint64_t SIM_get_count_chnlcfg_buf(SIM_chnlcfg_buf_t *buf)
{
	return buf->count;
}

SIM_chnlcfg_t *SIM_get_index_chnlcfg(SIM_chnlcfg_buf_t *buf, uint64_t i)
{
	if(i >= buf->count)
		return NULL;
	return &buf->cfgs[i];
}

void SIM_free_chnlcfg_buf(SIM_chnlcfg_buf_t *buf)
{
	free(buf->cfgs);
	free(buf);
}