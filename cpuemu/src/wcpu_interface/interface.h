#ifndef __WCPU_INTERFACE_HEADER__
#define __WCPU_INTERFACE_HEADER__

#include "message.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct interface
{
	message_t *message;
}interface_t;

#endif