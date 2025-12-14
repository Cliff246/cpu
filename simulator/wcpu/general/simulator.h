#ifndef __SIMULATOR_HEADER__
#define __SIMULATOR_HEADER__



#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


typedef enum simulator_type
{
	SIMULATOR_UNKNOWN,
	SIMULATOR_EMULATOR,
	SIMULATOR_VIRTUAL_MACHINE,
}simulator_type_t;








typedef struct simulator
{
	simulator_type_t type;


}simulator_t;


#endif