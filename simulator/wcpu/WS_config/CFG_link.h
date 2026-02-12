#ifndef __CFG_LINK_HEADER__
#define __CFG_LINK_HEADER__

#include "CFG_commons.h"
#include "SIM_channel.h"
#include "SIM_commons.h"
#include <stdint.h>
#include <sys/types.h>

typedef struct CFG_node CFG_node_t;
typedef struct CFG_edge CFG_edge_t;




//channel configure
typedef struct CFG_link
{
	//wire id
	SIM_wireid_t wireid;
	//channel id
	SIM_chnlid_t chnlid;

	//wirecfg
	CFG_edge_t *edge;
	//device configure
	CFG_node_t *devcfg;
}CFG_link_t;



typedef struct CFG_link_buf
{
	SIM_chnlid_t baseid;
	uint64_t count;
	uint64_t alloca;
	CFG_link_t *cfgs;

}CFG_link_buf_t;

void CFG_print_link(CFG_link_t *plink);




void CFG_append_link_buf(CFG_link_buf_t *buf, CFG_node_t *pnode, CFG_edge_id_t *eid, uint64_t count, CFG_link_id_t *rbuf);
uint64_t CFG_get_count_link_buf(CFG_link_buf_t *buf);

CFG_link_t *CFG_get_index_link_buf(CFG_link_buf_t *lbuf, uint64_t i);
CFG_link_buf_t *CFG_init_link_buf(uint64_t alloca);

void CFG_free_link_buf(CFG_link_buf_t *lbuf);

#endif