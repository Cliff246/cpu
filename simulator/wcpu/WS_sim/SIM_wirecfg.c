#include "SIM_wirecfg.h"

#include <stdio.h>


void SIM_init_wirecfg(SIM_wirecfg_t *ptr, uint64_t id, uint64_t latency, uint64_t throughput)
{
	ptr->id = id;
	ptr->latency = latency;
	ptr->throughput = throughput;
}

void SIM_wireconfig_print(SIM_wirecfg_t *wireconfig)
{
	printf("id: %ld latency: %ld throughput:%ld\n", wireconfig->id, wireconfig->latency, wireconfig->throughput);
}