#ifndef __WS_SIM_WIRECONFIG_HEADER__
#define __WS_SIM_WIRECONFIG_HEADER__

#include <stdint.h>

typedef struct WS_SIM_wireconfig
{
	uint64_t id;
	uint64_t latency;
}SIM_wireconfig_t;

void SIM_wireconfig_print(SIM_wireconfig_t *wireconfig);


#endif