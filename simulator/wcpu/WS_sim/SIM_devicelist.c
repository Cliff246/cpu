#include "SIM_devicelist.h"
#include "SIM_device.h"
#include <assert.h>
#include <stdlib.h>




SIM_devicelist_t *SIM_init_devicelist(uint64_t size)
{
	assert(size != 0);

	SIM_devicelist_t *devlist = calloc(1, sizeof(SIM_devicelist_t));
	assert(devlist);

	devlist->size = size;

	devlist->devices = calloc(size, sizeof(SIM_device_t ));
	assert(devlist->devices);

	devlist->done = false;
	return devlist;
}
SIM_device_t *SIM_get_device_devicelist(SIM_devicelist_t *devlist, uint64_t i)
{
	assert(devlist);

	if(i > devlist->size)
	{

		assert(0);

		return NULL;
	}


	return &devlist->devices[i];


}


void SIM_devicelist_print_all(SIM_devicelist_t *devlist)
{
	assert(devlist);
	for(uint64_t i = 0; i < devlist->size; ++i)
	{
		SIM_device_t *device = &devlist->devices[i];
		SIM_device_print(device);
	}
}

uint64_t SIM_get_devicelist_size(SIM_devicelist_t *devlist)
{
	return devlist->size;
}