/*

#include "device_message.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include "device_commons.h"
/*
char *device_message_type_string[] =
{
	DEVICE_MESSAGE_TYPE_LIST(DEVICE_MESSAGE_TYPE_STRING)
};

WS_dev_msg_t *WS_device_message_create(WS_dev_desc_t *src_type, WS_dev_id_t src_id, WS_dev_id_t dest_id, WS_dev_msg_type_t type, uint64_t address, int64_t value)
{
	dev_msg_t *msg = calloc(1, sizeof(dev_msg_t));
	assert(msg != NULL && "failed calloc");

	assert(type != DEVMSG_INVAL && "cannot create message with invalid type");
	assert(type != DEVMSG_READ_RESPOND && "cannot create message with a read respond type");
	msg->src_type = src_type;
	msg->dst_id = dest_id;
	msg->src_id = src_id;
	msg->address = address;
	msg->value = value;
	msg->ref_count = 1;
	msg->type = type;
	//desgined to allow unknown destinations to work, should route through the address resolution
	if(dest_id < 0)
	{
		msg->has_dst = false;
	}
	else
	{

		msg->has_dst = true;
	}


	return msg;
}


void WS_device_message_respond(WS_dev_msg_t *msg, int64_t value)
{
	assert(msg != NULL && "cannot respond to null message");

	assert(msg->type == DEVMSG_READ_SEND && "cannot respond to non read send msg");



	msg->value = value;
	msg->type = DEVMSG_READ_RESPOND;


}

uint64_t WS_get_device_message_address(WS_dev_msg_t *msg)
{
	assert(msg != NULL && "cannot respond to null message");

	return msg->address;
}


WS_dev_msg_type_t WS_get_device_message_type(WS_dev_msg_t *msg)
{
	assert(msg != NULL && "cannot get is_response from null message");
	return msg->type;
}


bool WS_get_device_message_has_dst(WS_dev_msg_t *msg)
{
	assert(msg != NULL && "cannot get is_response from null message");
	return msg->has_dst;
}

WS_dev_id_t WS_get_device_message_src_id(WS_dev_msg_t *msg)
{
	assert(msg != NULL && "cannot get src_id from null message");
	assert(msg->src_id != -1 && "src id can never be -1");

	return msg->src_id;
}

WS_dev_id_t WS_get_device_message_dst_id(WS_dev_msg_t *msg)
{
	assert(msg != NULL && "cannot get dst_id from null message");
	return msg->dst_id;

}


void WS_print_device_message(WS_dev_msg_t *msg)
{
	if(msg == NULL)
		return;
	printf("ptr:%p src_id:%d, dst_id:%d, has_dst:%d, msg_type: %s address: %lld, value: %lld, refcount:%d",msg, msg->src_id, msg->dst_id,  msg->has_dst, "empty for now", msg->address, msg->value, msg->ref_count);
}

bool WS_device_message_consume(WS_dev_msg_t **msg)
{
	assert(msg != NULL && "cannot get consumee from null message");
	assert(*msg != NULL && "cannot get message from bad reference");
	if(*msg == NULL)
		return false;
	(*msg)->ref_count++;
	return true;
}

bool WS_device_message_release(WS_dev_msg_t **msg)
{
	assert(msg != NULL && "cannot get dst_id from null message");
	assert(*msg != NULL && "cannot get message from bad reference");

	(*msg)->ref_count--;
	printf("free %p %d\n", *msg, (*msg)->ref_count);

	if((*msg)->ref_count <= 0)
	{
		free(*msg);
		*msg = NULL;
		return false;
	}
	*msg = NULL;
	return true;
}

*/