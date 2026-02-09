#ifndef __WS_SIM_DEVICECFG_HEADER__
#define __WS_SIM_DEVICECFG_HEADER__


#include "CFG_entry.h"
#include "SIM_channel.h"
#include "SIM_chnlcfg.h"
#include "SIM_commons.h"
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
	bool has_address;
	uint64_t address;
	uint64_t length;
	bool has_pretag;
	SIM_dtag_t pretag;
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
uint64_t SIM_devcfg_get_address_base(SIM_devcfg_t *device);
bool SIM_devcfg_has_address_range(SIM_devcfg_t *device);
uint64_t SIM_devcfg_get_address_size(SIM_devcfg_t *device);
SIM_dtag_t SIM_devcfg_has_pretag(SIM_devcfg_t *device);

#endif