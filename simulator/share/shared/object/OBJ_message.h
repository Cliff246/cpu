#ifndef __WS_OBJ_MESSAGE_HEADER__
#define __WS_OBJ_MESSAGE_HEADER__


#include <stdint.h>
#include <stdalign.h>
#include <stdbool.h>

#define OBJ_MESSAGE_SIZE 64
#define OBJ_MESSAGE_KEY_SIZE 16

typedef enum WS_OBJ_message_type
{
	OBJ_MSG_STORE,
	OBJ_MSG_LOAD,
	OBJ_MSG_TAG,
}OBJ_msgtype_t;

typedef struct WS_OBJ_message
{
	uint64_t head;

	uint8_t bytes_used;
	OBJ_msgtype_t type;
	_Alignas(64) uint8_t msg[OBJ_MESSAGE_SIZE];


}OBJ_msg_t;

#endif