#ifndef __DEVICE_MESSAGE_HEADER__
#define __DEVICE_MESSAGE_HEADER__

#include "device_type_ptr.h"
#include <stdint.h>
#include <stdbool.h>



typedef struct device_message
{
	int ref_count;
	//src device_type
	device_type_t src_type;
	//src id
	//dst id
	dev_id_t src_id, dst_id;


	uint64_t address;
	uint64_t value;
	bool sendback;

	bool is_response;


}dev_msg_t;


dev_msg_t *device_message_create(device_type_t src_type, dev_id_t src_id, dev_id_t dest_id, bool sendback, uint64_t address, uint64_t value);


void device_message_respond(dev_msg_t *msg, uint64_t value);

uint64_t get_device_message_address(dev_msg_t *msg);

bool get_device_message_sendback(dev_msg_t *msg);
bool get_device_message_is_response(dev_msg_t *msg);

dev_id_t get_device_message_src_id(dev_msg_t *msg);
dev_id_t get_device_message_dst_id(dev_msg_t *msg);

void print_device_message(dev_msg_t *msg);


dev_msg_t *device_message_consume(dev_msg_t *msg);
void device_message_release(dev_msg_t *msg);

#endif