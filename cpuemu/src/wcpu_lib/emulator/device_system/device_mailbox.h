#ifndef __DEVICE_MAILBOX_HEADER__
#define __DEVICE_MAILBOX_HEADER__



#include "device_message.h"
#include <stdbool.h>

#define DEVICE_MAILBOX_SIZE 5
typedef struct device_mailbox
{
	dev_msg_t *msg_ring[DEVICE_MAILBOX_SIZE];
	int write, read;

}dev_mailbox_t;

bool device_mailbox_put(dev_mailbox_t *mailbox, dev_msg_t *msg);
bool device_mailbox_get(dev_mailbox_t *mailbox, dev_msg_t **msg_ref);

dev_mailbox_t *device_mailbox_init(void);

void device_mailbox_print(dev_mailbox_t *mailbox);




#endif