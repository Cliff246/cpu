#ifndef __WS_SIM_CONNECTOR_HEADER__
#define __WS_SIM_CONNECTOR_HEADER__

#include <stdint.h>
#include <stdbool.h>
typedef struct WS_SIM_connector
{
	uint32_t latency;
	bool used;
}SIM_connector_t;

#endif