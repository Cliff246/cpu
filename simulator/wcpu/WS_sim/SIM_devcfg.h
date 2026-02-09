#ifndef __WS_SIM_DEVICECFG_HEADER__
#define __WS_SIM_DEVICECFG_HEADER__


#include "CFG_entry.h"
#include "SIM_channel.h"
#include "SIM_chnlcfg.h"
#include "hashmap.h"
#include "SIM_handle.h"

#include <stdbool.h>
#include <stdint.h>


typedef struct WS_SIM_devicecfg
{
	//deep copied over
	p_hashtable_t initals;
	char *module;
	uint64_t channels;
	struct
	{
		SIM_device_t *transition;
		bool finished;
	}done;

}SIM_devcfg_t;

bool SIM_devcfg_add_prototag(SIM_devcfg_t *ptr, CFG_prototag_t *tag);

bool SIM_devcfg_prep_chnlcfgs(SIM_devcfg_t *ptr,  SIM_chnlcfg_buf_t *buf);

bool SIM_init_devcfg_tags(SIM_devcfg_t *ptr, CFG_entry_t *entry);

bool SIM_init_devcfg(SIM_devcfg_t *ptr, CFG_entry_t *entry, SIM_chnlcfg_buf_t *buf);

void SIM_free_devcfg(SIM_devcfg_t *ptr);

#endif