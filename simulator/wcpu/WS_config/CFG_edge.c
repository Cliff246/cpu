#include "CFG_edge.h"
#include "CFG_link.h"
#include "CFG_commons.h"
#include "commons.h"

#include <stdint.h>
#include <stdio.h>



void CFG_init_edge(CFG_edge_t *edge, CFG_edge_id_t id, uint64_t latency, uint64_t throughput)
{
	edge->id = id;
	edge->latency = latency;
	edge->throughput = throughput;
	edge->done.added_channels = false;
}
void CFG_print_edge(CFG_edge_t *wireconfig)
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
void CFG_append_edge(CFG_edge_t  *wireconfig, CFG_node_id_t cid)
{
	wireconfig->done.added_channels = true;
	if(wireconfig->channels_alloc <= wireconfig->channels_count)
	{
		wireconfig->channels_alloc = (wireconfig->channels_alloc * 2 ) + 1;
		wireconfig->channels = realloc_safe(wireconfig->channels, wireconfig->channels_alloc, sizeof(CFG_link_id_t));

	}
	wireconfig->channels[wireconfig->channels_count ++] = cid;
}
void CFG_free_edge(CFG_edge_t *ptr)
{
	free(ptr->channels);

}