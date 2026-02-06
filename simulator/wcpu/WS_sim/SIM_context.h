#ifndef __WS_SIM_CONTEXT_HEADER__
#define __WS_SIM_CONTEXT_HEADER__

#include "SIM_channel.h"
#include "SIM_device.h"
#include "SIM_devicelist.h"
#include "CFG_manifest.h"
#include "SIM_wireconfig.h"

#include <stdlib.h>
#include <stdint.h>


typedef struct WS_SIM_context
{
	SIM_devicelist_t *devicelist;

	uint64_t wireconfigs_size;
	SIM_wireconfig_t *wireconfigs;

}SIM_context_t;

static SIM_devicelist_t *SIM_context_init_devicelist(CFG_manifest_t *manifest);
static void SIM_context_init_wireconfigs(CFG_manifest_t *manifest);
SIM_context_t *SIM_init_context(CFG_manifest_t *manifest);


#endif