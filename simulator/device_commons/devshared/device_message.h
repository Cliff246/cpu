#ifndef __DEVICE_MESSAGE_HEADER__
#define __DEVICE_MESSAGE_HEADER__

#include "device_list.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct device_description WS_dev_desc_t;


#define DEVICE_MESSAGE_TYPE_LIST(X) \
	X(INVAL)						\
	X(READ_SEND)					\
	X(READ_RESPOND)					\
	X(WRITE)						\

#define DEVICE_MESSAGE_TYPE_NAME(X) DEVMSG_ ## X
#define DEVICE_MESSAGE_TYPE_ENUM(X) DEVICE_MESSAGE_TYPE_NAME(X),

typedef enum device_message_type
{
	DEVICE_MESSAGE_TYPE_LIST(DEVICE_MESSAGE_TYPE_ENUM)

}dev_msg_type_t, WS_dev_msg_type_t;
#define DEVICE_MESSAGE_TYPE_NAME_STRING(X) TOSTR(DEVICE_MESSAGE_TYPE_NAME(X))
#define DEVICE_MESSAGE_TYPE_STRING(X) [DEVICE_MESSAGE_TYPE_NAME(X)] = DEVICE_MESSAGE_TYPE_NAME_STRING(X),

//extern char *device_message_type_string[];

typedef struct
{
	
}WS_dev_msg_block_t;



typedef struct wcpu_simulator_device_message
{
	int ref_count;
	//add an owner system

	WS_dev_id_t owner;
	//src device_type
	WS_dev_desc_t *src_type;
	//src id
	//dst id
	WS_dev_id_t src_id, dst_id;

	uint64_t address;
	int64_t value;


	dev_msg_type_t type;

	bool has_dst;



}dev_msg_t, WS_dev_msg_t;




#endif