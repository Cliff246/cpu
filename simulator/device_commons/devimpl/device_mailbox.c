
/*
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//returns false on cannot and returns true on success
bool WS_device_mailbox_put(WS_dev_mailbox_t *dev, WS_dev_msg_t *msg)
{
	WS_dev_mailbox_t *mb = dev;

	if(((mb->write + 1) % DEVICE_MAILBOX_SIZE) == mb->read)
	{
		//buffer is full
		return false;
	}

	mb->msg_ring[mb->write] = msg;
	mb->write = (mb->write + 1) % DEVICE_MAILBOX_SIZE;
	msg->ref_count++;
	return true;
}


bool WS_device_mailbox_pop(WS_dev_mailbox_t *dev, WS_dev_msg_t **msg)
{
	assert(dev);

	WS_dev_mailbox_t *mb = dev;
	if(mb->read == mb->write)
	{
		*msg = NULL;
		return false;
	}
	*msg = mb->msg_ring[mb->read];

	(*msg)->ref_count--;

	mb->msg_ring[mb->read] = NULL;

	mb->read = (mb->read + 1) % DEVICE_MAILBOX_SIZE;
	return true;

}

WS_dev_mailbox_t *WS_device_mailbox_init(void)
{
	WS_dev_mailbox_t *mailbox = calloc(1, sizeof(WS_dev_mailbox_t ));
	assert(mailbox != NULL && "mail box failed alloc");
	return mailbox;

}

void WS_device_mailbox_print(WS_dev_mailbox_t *mb)
{
	WS_dev_mailbox_t *mailbox = mb;
	assert(mailbox);
	for(int i = 0; i < DEVICE_MAILBOX_SIZE; ++i)
	{
		dev_msg_t *msg = mailbox->msg_ring[i];
		printf("[%d] = [", i);

		if(msg != NULL)
		{
			WS_print_device_message(msg);
		}
		printf("]\n");;

	}
}
*/