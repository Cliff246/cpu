#include "device_mailbox.h"
#include "device_commons.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//returns false on cannot and returns true on success
bool device_mailbox_put(dev_mailbox_t *mailbox, dev_msg_t *msg)
{
	assert(mailbox && msg);

	if(((mailbox->write + 1) % DEVICE_MAILBOX_SIZE) == mailbox->read)
	{
		//buffer is full
		return false;
	}

	mailbox->msg_ring[mailbox->write] = msg;
	mailbox->write = (mailbox->write + 1) % DEVICE_MAILBOX_SIZE;
	msg->ref_count++;
	return true;
}


//mail box returns false on nothing to read and true on something read
bool device_mailbox_get(dev_mailbox_t *mailbox, dev_msg_t **msg_ref)
{
	assert(mailbox && msg_ref);
	if(mailbox->read == mailbox->write)
	{
		*msg_ref = NULL;
		return false;
	}
	*msg_ref = mailbox->msg_ring[mailbox->read];

	(*msg_ref)->ref_count--;

	mailbox->msg_ring[mailbox->read] = NULL;

	mailbox->read = (mailbox->read + 1) % DEVICE_MAILBOX_SIZE;
	return true;

}

dev_mailbox_t *device_mailbox_init(void)
{
	dev_mailbox_t *mailbox = calloc(1, sizeof(dev_mailbox_t ));
	assert(mailbox != NULL && "mail box failed alloc");
	return mailbox;

}

void device_mailbox_print(dev_mailbox_t *mailbox)
{
	assert(mailbox);
	for(int i = 0; i < DEVICE_MAILBOX_SIZE; ++i)
	{
		dev_msg_t *msg = mailbox->msg_ring[i];
		printf("[%d] = [", i);

		if(msg != NULL)
		{
			print_device_message(msg);
		}
		printf("]\n");;

	}
}