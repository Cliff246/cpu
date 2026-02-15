#ifndef __WS_OBJ_BUNDLE_HEADER__
#define __WS_OBJ_BUNDLE_HEADER__

#include "OBJ_locals.h"
#include "OBJ_message.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>



typedef struct WS_OBJ_bundle
{
	uint64_t size;
	OBJ_msg_t *msgs;
}OBJ_bndl_t;

#endif