#ifndef __DEVICE_MESSAGE_HEADER__
#define __DEVICE_MESSAGE_HEADER__

#include "device_list.h"
#include "common.h"
#include <stdint.h>
#include <stdbool.h>

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

}dev_msg_type_t;
#define DEVICE_MESSAGE_TYPE_NAME_STRING(X) TOSTR(DEVICE_MESSAGE_TYPE_NAME(X))
#define DEVICE_MESSAGE_TYPE_STRING(X) [DEVICE_MESSAGE_TYPE_NAME(X)] = DEVICE_MESSAGE_TYPE_NAME_STRING(X),

extern char *device_message_type_string[];

typedef struct device_message
{
	int ref_count;
	//add an owner system

	dev_id_t owner;
	//src device_type
	device_type_t src_type;
	//src id
	//dst id
	dev_id_t src_id, dst_id;

	uint64_t address;
	int64_t value;


	dev_msg_type_t type;

	bool has_dst;



}dev_msg_t;


dev_msg_t *device_message_create(device_type_t src_type, dev_id_t src_id, dev_id_t dest_id, dev_msg_type_t type, uint64_t address, int64_t value);


void device_message_respond(dev_msg_t *msg, int64_t value);

uint64_t get_device_message_address(dev_msg_t *msg);


dev_msg_type_t get_device_message_type(dev_msg_t *msg);
bool get_device_message_has_dst(dev_msg_t *msg);


dev_id_t get_device_message_src_id(dev_msg_t *msg);
dev_id_t get_device_message_dst_id(dev_msg_t *msg);

void print_device_message(dev_msg_t *msg);

bool device_message_consume(dev_msg_t **msg);
bool device_message_release(dev_msg_t **msg);


#endif