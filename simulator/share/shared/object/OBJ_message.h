#ifndef __WS_OBJ_MESSAGE_HEADER__
#define __WS_OBJ_MESSAGE_HEADER__


#include <stdint.h>
#include <stdalign.h>
#include <stdbool.h>

#define OBJ_MESSAGE_SIZE 64
#define OBJ_MESSAGE_KEY_SIZE 16

typedef int32_t OBJ_msgtag_t;

typedef enum WS_OBJ_message_type
{
	OBJ_MSG_INVAL,
	OBJ_MSG_STORE,
	OBJ_MSG_LOAD,
	OBJ_MSG_TAG,
}OBJ_msgtype_t;


typedef union WS_OBJ_message_value
{
	uint64_t address;
	OBJ_msgtag_t tag;
}OBJ_msgval_t;


typedef struct WS_OBJ_message
{
	OBJ_msgval_t value;
	OBJ_msgtag_t tag;
	uint8_t bytes_used;
	OBJ_msgtype_t type;
	_Alignas(64) uint8_t msg[OBJ_MESSAGE_SIZE];


}OBJ_msg_t;

#endif