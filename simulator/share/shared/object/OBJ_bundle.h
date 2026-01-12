#ifndef __WS_OBJ_BUNDLE_HEADER__
#define __WS_OBJ_BUNDLE_HEADER__

#include "OBJ_message.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>



typedef struct WS_OBJ_msgbnd
{
	const uint32_t size;
	uint32_t used;

	_Alignas(8) OBJ_msg_t msgs[];

}OBJ_bnd_t;

#endif