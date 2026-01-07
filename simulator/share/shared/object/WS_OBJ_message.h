#ifndef __OBJECT_MESSAGE_HEADER__
#define __OBJECT_MESSAGE_HEADER__

#include "flags.h"

#include <stdint.h>
#include <stdbool.h>

#include <stdbool.h>
#include <stdalign.h>

#define WS_OBJ_MESSAGE_HEAD_TYPE_SIZE 6
#define WS_OBJ_MESSAGE_BODY_LINE_SIZE 8



typedef struct WS_OBJ_messsage_body
{
	_Alignas(2) uint8_t line[WS_OBJ_MESSAGE_BODY_LINE_SIZE];
}OBJ_message_body_t;

typedef struct WS_OBJ_message_head
{
	uint16_t tag;
	uint8_t type[WS_OBJ_MESSAGE_HEAD_TYPE_SIZE];

}OBJ_message_head_t;

typedef struct WS_OBJ_message
{
	enum
	{
		OBJ_MESSAGE_HEAD,
		OBJ_MESSAGE_BODY,
	}type;
	union
	{
		OBJ_message_head_t head;
		OBJ_message_body_t body;
	}msg;
}OBJ_message_t;


#endif
