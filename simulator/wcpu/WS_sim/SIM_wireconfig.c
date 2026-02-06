#include "SIM_wireconfig.h"

#include <stdio.h>

void SIM_wireconfig_print(SIM_wireconfig_t *wireconfig)
{
	printf("id: %ld latency: %ld\n", wireconfig->id, wireconfig->latency);
}