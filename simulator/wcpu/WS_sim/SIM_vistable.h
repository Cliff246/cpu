#ifndef __WS_SIM_VISTABLE_HEADER__
#define __WS_SIM_VISTABLE_HEADER__

#include "SIM_channel.h"
#include "SIM_chnlcfg.h"
#include "SIM_commons.h"
#include <stdint.h>

typedef struct WS_SIM_context SIM_context_t;

typedef struct WS_SIM_viselm
{
	SIM_devcfg_t *device;
	bool has_address;
	uint64_t address;
	uint64_t length;
	bool pretag;
	SIM_dtag_t tag;

}SIM_viselm_t;

SIM_dtag_t SIM_init_viselm(SIM_viselm_t *elm, SIM_devcfg_t *device);



typedef struct WS_SIM_vistable
{
	uint64_t count;
	SIM_viselm_t *table;

	SIM_viselm_t **tagmap;

}SIM_vistable_t;

SIM_vistable_t *SIM_init_vistable(SIM_context_t *ctx);

#endif