#include "device_message.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

dev_msg_t *device_message_create(device_type_t src_type, dev_id_t src_id, dev_id_t dest_id, bool sendback, uint64_t address, uint64_t value)
{
	dev_msg_t *msg = calloc(1, sizeof(dev_msg_t));
	assert(msg != NULL && "failed calloc");

	msg->src_type = src_type;
	msg->dst_id = dest_id;
	msg->src_id = src_id;
	msg->sendback = sendback;
	msg->address = address;
	msg->value = value;
	msg->is_response = false;
	msg->ref_count = 1;
	return msg;
}


void device_message_respond(dev_msg_t *msg, uint64_t value)
{
	assert(msg != NULL && "cannot respond to null message");

	assert(msg->sendback != true && "cannot respond to non sendback msg");

	assert(msg->is_response != true && "cannot respond to message thats already been responded too");


	msg->value = value;
	msg->is_response = true;


}

uint64_t get_device_message_address(dev_msg_t *msg)
{
	assert(msg != NULL && "cannot respond to null message");

	return msg->address;
}


bool get_device_message_sendback(dev_msg_t *msg)
{
	assert(msg != NULL && "cannot get sendback from null message");
	return msg->sendback;

}

bool get_device_message_is_response(dev_msg_t *msg)
{
	assert(msg != NULL && "cannot get is_response from null message");
	return msg->is_response;

}

dev_id_t get_device_message_src_id(dev_msg_t *msg)
{
	assert(msg != NULL && "cannot get src_id from null message");
	assert(msg->src_id != -1 && "src id can never be -1");

	return msg->src_id;
}

dev_id_t get_device_message_dst_id(dev_msg_t *msg)
{
	assert(msg != NULL && "cannot get dst_id from null message");
	assert(msg->dst_id != -1 && "src id can never be -1");
	return msg->dst_id;

}


void print_device_message(dev_msg_t *msg)
{
	if(msg == NULL)
		return;
	printf("src_id:%d, dst_id:%d, is_response:%d, sendback:%d, address: %lld, value: %lld\n", msg->src_id, msg->dst_id, msg->is_response, msg->sendback, msg->address, msg->value);
}

dev_msg_t *device_message_consume(dev_msg_t *msg)
{
	assert(msg != NULL && "cannot get dst_id from null message");
	msg->ref_count++;
	return msg;
}

void device_message_release(dev_msg_t *msg)
{
	assert(msg != NULL && "cannot get dst_id from null message");
	msg->ref_count--;
	if(msg->ref_count <= 0)
		free(msg);
}