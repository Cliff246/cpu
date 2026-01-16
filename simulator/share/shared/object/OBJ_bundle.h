#ifndef __WS_OBJ_BUNDLE_HEADER__
#define __WS_OBJ_BUNDLE_HEADER__

#include "OBJ_message.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>


#define OBJ_BUNDLE_SIZE 8

typedef struct WS_OBJ_bundle
{

	_Alignas(128) OBJ_msg_t msg[OBJ_BUNDLE_SIZE];

}OBJ_bundle_t;

#endif