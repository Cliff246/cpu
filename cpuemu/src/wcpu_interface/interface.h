#ifndef __WCPU_INTERFACE_HEADER__
#define __WCPU_INTERFACE_HEADER__

#include "message.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>



typedef enum interface_type
{
	INTERFACE_INVALID,
	INTERFACE_VM,
	INTERFACE_EMU,
}interface_type_t;

typedef struct interface
{
	interface_type_t type;

	void *machine;





}interface_t;






#endif