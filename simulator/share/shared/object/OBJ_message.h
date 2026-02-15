#ifndef __OBJ_MESSAGE_HEADER__
#define __OBJ_MESSAGE_HEADER__


#include <stdint.h>
#include <stdalign.h>
#include <stdbool.h>

#define OBJ_MSG_BLK_SIZE 64


typedef struct OBJ_message_block
{
	_Alignas(8) uint8_t block[OBJ_MSG_BLK_SIZE];
}OBJ_msgblk_t;


typedef enum OBJ_message_control_type
{
	OBJ_MSGCTRL_INVAL,
	OBJ_MSGCTRL_LOAD,
	OBJ_MSGCTRL_STORE,
}OBJ_msgctrl_type_t;

typedef enum OBJ_message_header_type
{
	OBJ_MSGHEAD_INVAL,
	OBJ_MSGHEAD_TAG,
	OBJ_MSGHEAD_ADDRESS,
}OBJ_msghead_type_t;

typedef union OBJ_message_header_value
{
	uint64_t address;
	int64_t tag;
}OBJ_msghead_val_t;

typedef struct OBJ_message_header
{
	OBJ_msghead_type_t type;
	OBJ_msghead_val_t val;

}OBJ_msghead_t;

typedef struct OBJ_message_control
{
	OBJ_msgctrl_type_t type;
	uint32_t arg;
}OBJ_msgctrl_t;

typedef struct OBJ_message
{
	OBJ_msgctrl_t ctrl;
	OBJ_msghead_t dst;
	OBJ_msghead_t src;

	uint64_t bits;
	uint64_t count;

	_Alignas(64) OBJ_msgblk_t blocks[];
}OBJ_msg_t;

#endif