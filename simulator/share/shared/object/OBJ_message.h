#ifndef __WS_OBJ_MESSAGE_HEADER__
#define __WS_OBJ_MESSAGE_HEADER__


#include <stdint.h>
#include <stdalign.h>
#include <stdbool.h>

#define OBJ_MESSAGE_SIZE 64
#define OBJ_MESSAGE_KEY_SIZE 16
typedef struct WS_OBJ_message
{
	_Alignas(64) uint8_t msg[OBJ_MESSAGE_SIZE];
	char key[OBJ_MESSAGE_KEY_SIZE];
	uint64_t group;
	int16_t used;
	uint8_t index;
	uint8_t size;
}OBJ_msg_t;

#endif