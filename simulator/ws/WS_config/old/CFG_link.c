#include "CFG_link.h"
#include "CFG_commons.h"
#include "commons.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*

void CFG_print_link(CFG_link_t *plink)
{
	printf("devcfg:%p edge:%p, wireid: %ld, chnlid: %ld\n", plink->devcfg, plink->edge, plink->wireid, plink->chnlid);
}

//args:
//takes the channel config buffer,
//a device configure ptr,
//a wire id array of size count,
//sisze of count,
//a buffer of channel ids of the same size count,
void CFG_append_link_buf(CFG_link_buf_t *buf, CFG_node_t *pnode, CFG_edge_id_t *eid, uint64_t count, CFG_link_id_t *rbuf)
{
	assert(count != 0);
	if(buf->alloca < buf->count + count)
	{
		const uint64_t diff = (buf->count + count) - buf->alloca;
		uint64_t alloca = buf->alloca + diff;
		buf->cfgs = realloc_safe(buf->cfgs, alloca, sizeof(CFG_link_t));
		assert(buf->cfgs);
		buf->alloca = alloca;
	}

	for(uint64_t i = 0; i < count; ++i)
	{

		CFG_link_id_t cid = buf->baseid++;
		rbuf[i] = cid;
		CFG_link_t config =
		{
			.devcfg = pnode,
			.wireid = eid[i],
			.chnlid = cid,
			.edge = NULL,
		};

		buf->cfgs[buf->count++] = config;
	}
	return;
}
CFG_link_buf_t *CFG_init_link_buf(uint64_t alloca)
{
	if(alloca == 0)
		alloca = 10;
	CFG_link_t *cfgs = calloc(alloca, sizeof(CFG_link_t));
	assert(cfgs && "calloc cfgs");
	CFG_link_buf_t *lbuf = calloc(1, sizeof(CFG_link_buf_t));
	assert(lbuf);

	lbuf->count = 0;

	lbuf->alloca = alloca;
	lbuf->cfgs = cfgs;
	return lbuf;

}
uint64_t CFG_get_count_link_buf(CFG_link_buf_t *buf)
{
	return buf->count;
}
CFG_link_t *CFG_get_index_link_buf(CFG_link_buf_t *lbuf, uint64_t i)
{
	if(i >= lbuf->count)
		return NULL;
	return &lbuf->cfgs[i];
}
void CFG_free_link_buf(CFG_link_buf_t *lbuf)
{
	free(lbuf->cfgs);
	free(lbuf);
}

*/