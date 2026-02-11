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
#include <sys/types.h>
#include <stdalign.h>


typedef struct WS_SIM_devcfg_route
{
	uint64_t latency;
	SIM_wireid_t wire;
}SIM_devcfg_route_t;

typedef struct WS_SIM_devcfg_routerow
{
	SIM_dtag_t tag; //target device identity
	uint64_t size; //size of the valid routes
	//per valid devconfig
	SIM_devcfg_route_t *routes; //the routes
}SIM_devcfg_routerow_t;


typedef struct WS_SIM_devcfg_routetable
{
	uint64_t tags_size; //the amount of devices
	//per device id
	SIM_devcfg_routerow_t *tags; //the device route 
}SIM_devcfg_routetable_t;


 typedef struct WS_SIM_devicecfg
{
	//deep copied over
	_Alignas(128) p_hashtable_t initals;
	char *module;
	uint64_t chnlids_size;
	SIM_chnlid_t *chnlids;
	bool has_address;
	uint64_t address;
	uint64_t length;
	bool has_pretag;
	SIM_dtag_t pretag;
	SIM_devcfg_routetable_t routetable;
	struct
	{
		SIM_device_t *transition;
		bool init_routetable;
		bool finished;
	}done;

}SIM_devcfg_t;

bool SIM_devcfg_add_prototag(SIM_devcfg_t *ptr, CFG_prototag_t *tag);

bool SIM_devcfg_prep_chnlcfgs(SIM_devcfg_t *ptr,  SIM_chnlcfg_buf_t *buf);

bool SIM_init_devcfg_tags(SIM_devcfg_t *ptr, CFG_entry_t *entry);

bool SIM_init_devcfg(SIM_devcfg_t *ptr, CFG_entry_t *entry, SIM_chnlcfg_buf_t *buf);
void SIM_free_devcfg(SIM_devcfg_t *dev);

uint64_t SIM_devcfg_get_address_base(SIM_devcfg_t *device);
bool SIM_devcfg_has_address_range(SIM_devcfg_t *device);
uint64_t SIM_devcfg_get_address_size(SIM_devcfg_t *device);
SIM_dtag_t SIM_devcfg_has_pretag(SIM_devcfg_t *device);
void SIM_init_routetable_devcfg(SIM_devcfg_t *devcfg, uint64_t tags_size, SIM_dtag_t *dtags);
void SIM_append_route_devcfg(SIM_devcfg_t *devcfg, uint64_t index, SIM_devcfg_route_t route);




#endif