#include "device_message.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

dev_msg_t *device_message_create(device_type_t src_type, dev_id_t src_id, dev_id_t dest_id, dev_msg_type_t type, dev_msg_content_t content)
{
	dev_msg_t *msg = calloc(1, sizeof(dev_msg_t));
	if(msg == NULL)
	{
		perror("cannot create message");
		exit(EXIT_FAILURE);
	}

	msg->src_type = src_type;
	msg->dst_id = dest_id;
	msg->src_id = src_id;
	msg->msg_type = type;
	msg->msg = content;
	msg->ref_count = 1;
	return msg;
}