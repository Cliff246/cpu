#ifndef __SIMULATOR_HEADER__
#define __SIMULATOR_HEADER__


#include "emulator.h"
#include "vm.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct simulator_emulator
{
	emulator_t *emu;
}simulator_emu_t;

typedef struct simulator_virtual_machine
{
	vima_t *vima;
}simulator_vima_t;

typedef enum simulator_type
{
	SIMULATOR_UNKNOWN,
	SIMULATOR_EMULATOR,
	SIMULATOR_VIRTUAL_MACHINE,
}simulator_type_t;








typedef struct simulator
{
	simulator_type_t type;

	union
	{
		simulator_vima_t vm;
		simulator_emu_t emu;
	}sim;

}simulator_t;


#endif