#include "SIM_context.h"
#include "CFG_manifest.h"

#include "SIM_context.h"
#include "SIM_device.h"
#include "SIM_devicelist.h"
#include "SIM_wireconfig.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>



static SIM_devicelist_t *SIM_context_init_devicelist(CFG_manifest_t *manifest)
{
	const uint64_t size = manifest->size;
	assert(size > 0);
	SIM_devicelist_t *devlist = SIM_init_devicelist(size);
	assert(devlist);
	for(int i = 0; i < manifest->size; ++i)
	{
		CFG_entry_t *entry = manifest->entries[i];
		SIM_device_t *device = SIM_get_device_devicelist(devlist, i);
		SIM_init_device(device, entry);
		assert(device);

	}
	return devlist;
}




SIM_context_t *SIM_init_context(CFG_manifest_t *manifest)
{
	SIM_context_t *ctx = calloc(1, sizeof(SIM_context_t));
	assert(ctx);

	ctx->devicelist = SIM_context_init_devicelist(manifest);
	const uint64_t size = manifest->settings_size;
	assert(size > 0);
	//printf("size: %d\n", size);
	SIM_wireconfig_t *wireconfigs = calloc(size, sizeof(SIM_wireconfig_t));
	assert(wireconfigs);

	for(uint64_t i = 0; i < size; ++i)
	{
		CFG_setting_t *setting = manifest->settings[i];

		SIM_wireconfig_t connector =
		{
			.id = setting->id,
			.latency = setting->latency,
		};

		wireconfigs[i] = connector;
		SIM_wireconfig_print(&connector);
	}
	ctx->wireconfigs = wireconfigs;
	ctx->wireconfigs_size = size;
	return ctx;
}