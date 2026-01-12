#ifndef __WS_OBJ_MESSAGE_HEADER__
#define __WS_OBJ_MESSAGE_HEADER__

#include "OBJ_input.h"
#include "OBJ_output.h"

#include <stdint.h>
#include <stdalign.h>
#include <stdbool.h>

#define OBJ_MSG_TYPE_SIZE 6

typedef enum WS_OBJ_message_direction
{
	OBJ_MSG_IN,
	OBJ_MSG_OUT,
}OBJ_msgdir_t;

typedef union WS_OBJ_message_implementation
{
	OBJ_msgin_t in;
	OBJ_msgout_t out;
}OBJ_msgimpl_t;

typedef struct WS_OBJ_message
{
	OBJ_msgdir_t dir;
	uint16_t pid;
	uint8_t type[6];
	_Alignas(8) OBJ_msgimpl_t msg;
}OBJ_msg_t;

#endif