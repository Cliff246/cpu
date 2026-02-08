#ifndef __WS_SIM_ENVELOPE_HEADER__
#define __WS_SIM_ENVELOPE_HEADER__

#include <stdint.h>

typedef struct WS_SIM_envelope
{
	uint64_t address;
	uint64_t sender;
}SIM_envelope_t;

#endif