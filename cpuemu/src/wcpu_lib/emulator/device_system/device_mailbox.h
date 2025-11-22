#ifndef __DEVICE_MAILBOX_HEADER__
#define __DEVICE_MAILBOX_HEADER__



#include "device_message.h"

#define DEVICE_MAILBOX_SIZE 20

typedef struct device_mailbox
{
	dev_msg_t *msg_ring[DEVICE_MAILBOX_SIZE];
	int write, read;

}dev_mailbox_t;

int device_mailbox_put(dev_mailbox_t *mailbox, dev_msg_t *msg);
int device_mailbox_get(dev_mailbox_t *mailbox, dev_msg_t **msg_ref);

void device_mailbox_init(dev_mailbox_t *mailbox);





#endif