#include "SIM_wirecfg.h"
#include "SIM_chnlcfg.h"
#include "SIM_commons.h"
#include "commons.h"

#include <stdint.h>
#include <stdio.h>

void SIM_init_wirecfg(SIM_wirecfg_t *ptr, SIM_wireid_t id, uint64_t latency, uint64_t throughput)
{
	ptr->id = id;
	ptr->latency = latency;
	ptr->throughput = throughput;
	ptr->done.added_channels = false;
}

void SIM_print_wirecfg(SIM_wirecfg_t *wireconfig)
{


	printf("wirecfg <id: %ld latency: %ld throughput:%ld>\n", wireconfig->id, wireconfig->latency, wireconfig->throughput);
	if(wireconfig->done.added_channels == true)
	{
		for(uint64_t i = 0; i < wireconfig->channels_count; ++i)
		{
			printf("cid: %ld\n", wireconfig->channels[i]);
		}
	}
	printf("\n");
}

void SIM_append_wirecfg(SIM_wirecfg_t *wireconfig, SIM_chnlid_t cid)
{
	wireconfig->done.added_channels = true;
	if(wireconfig->channels_alloc <= wireconfig->channels_count)
	{
		wireconfig->channels_alloc = (wireconfig->channels_alloc * 2 ) + 1;
		wireconfig->channels = realloc_safe(wireconfig->channels, wireconfig->channels_alloc, sizeof(SIM_chnlid_t));

	}
	wireconfig->channels[wireconfig->channels_count ++] = cid;
}

void SIM_free_wirecfg(SIM_wirecfg_t *ptr)
{
	free(ptr->channels);

}