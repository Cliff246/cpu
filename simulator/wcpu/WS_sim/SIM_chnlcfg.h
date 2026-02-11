#ifndef __WS_SIM_CHNLCFG_HEADER__
#define __WS_SIM_CHNLCFG_HEADER__

#include "SIM_channel.h"
#include "SIM_commons.h"
#include <stdint.h>
#include <sys/types.h>

typedef struct WS_SIM_devicecfg SIM_devcfg_t;
typedef struct WS_SIM_wirecfg SIM_wirecfg_t;




//channel configure
typedef struct WS_SIM_chnlcfg
{
	//wire id
	SIM_wireid_t wireid;
	//channel id
	SIM_chnlid_t chnlid;

	//wirecfg
	SIM_wirecfg_t *wirecfg;
	//device configure
	SIM_devcfg_t *devcfg;
}SIM_chnlcfg_t;



typedef struct WS_SIM_chnlcfg_buf
{
	SIM_chnlid_t baseid;
	uint64_t count;
	uint64_t alloca;
	SIM_chnlcfg_t *cfgs;

}SIM_chnlcfg_buf_t;

void SIM_print_chnlcfg(SIM_chnlcfg_t *cfg);




void SIM_append_chnlcfg_buf(SIM_chnlcfg_buf_t *buf, SIM_devcfg_t *devcfg, SIM_wireid_t *wid, uint64_t count, SIM_chnlid_t *rbuf);

uint64_t SIM_get_count_chnlcfg_buf(SIM_chnlcfg_buf_t *buf);
SIM_chnlcfg_t *SIM_get_index_chnlcfg(SIM_chnlcfg_buf_t *buf, uint64_t i);
SIM_chnlcfg_buf_t *SIM_init_chnlcfg_buf(uint64_t alloca);
void SIM_free_chnlcfg_buf(SIM_chnlcfg_buf_t *buf);

#endif