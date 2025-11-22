#ifndef __DEVICE_MESSAGE_HEADER__
#define __DEVICE_MESSAGE_HEADER__

#include "device_type_ptr.h"
#include <stdint.h>
#include <stdbool.h>

#define DEVICE_MESSAGE_LIST(X)	\
	X(INVAL)					\
	X(WRITE)					\
	X(READ)						\

#define DEVICE_MESSAGE_ENUM_RESOLVE(name) DEVICE_MESSAGE_ ## name

#define DEVICE_MESSAGE_ENUM_NAME(X) DEVICE_MESSAGE_ENUM_RESOLVE(X),


typedef enum device_message_type
{
	DEVICE_MESSAGE_LIST(DEVICE_MESSAGE_ENUM_NAME)

}dev_msg_type_t;


typedef struct device_message_content_write
{
	uint64_t address;
	uint64_t value;
}dev_msg_content_write;

typedef struct device_message_content_read
{
	uint64_t address;
}dev_msg_content_read;


typedef union device_message_content
{
	dev_msg_content_write write;
	dev_msg_content_read read;
}dev_msg_content_t;


typedef struct device_message
{
	int ref_count;
	//src device_type
	device_type_t src_type;
	//src id
	//dst id
	dev_id_t src_id, dst_id;

	//msg type
	dev_msg_type_t msg_type;

	dev_msg_content_t msg;

}dev_msg_t;


dev_msg_t *device_message_create(device_type_t src_type, dev_id_t src_id, dev_id_t dest_id, dev_msg_type_t type, dev_msg_content_t content);

#endif