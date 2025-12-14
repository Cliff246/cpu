#ifndef __DEVICE_MESSAGE_IMPL_HEADER__
#define __DEVICE_MESSAGE_IMPL_HEADER__

#include "device.h"
#include "device_message.h"

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