#include "device_mailbox.h"

#include <stdlib.h>
#include <string.h>

//returns 1 on cannot and returns 0 on success
int device_mailbox_put(dev_mailbox_t *mailbox, dev_msg_t *msg)
{

	if(((mailbox->write + 1) % DEVICE_MAILBOX_SIZE) == mailbox->read)
	{
		//buffer is full
		return 1;
	}

	mailbox->msg_ring[mailbox->write] = msg;
	mailbox->write = (mailbox->write + 1) % DEVICE_MAILBOX_SIZE;
	msg->ref_count++;
	return 0;
}


//mail box returns 1 on nothing to read and 0 on something read
int device_mailbox_get(dev_mailbox_t *mailbox, dev_msg_t **msg_ref)
{
	if(mailbox->read == mailbox->write)
	{
		*msg_ref = NULL;
		return 1;
	}
	*msg_ref = mailbox->msg_ring[mailbox->read];
	(*msg_ref)->ref_count--;
	mailbox->read = (mailbox->read + 1) % DEVICE_MAILBOX_SIZE;
	return 0;

}


void device_mailbox_init(dev_mailbox_t *mailbox)
{
	memset(mailbox->msg_ring, 0, sizeof(dev_msg_t *));
	

}

