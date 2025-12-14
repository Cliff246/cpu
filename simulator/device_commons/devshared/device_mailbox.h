#ifndef __DEVICE_MAILBOX_HEADER__
#define __DEVICE_MAILBOX_HEADER__



#include "device_message.h"
#include <stdbool.h>

#define DEVICE_MAILBOX_SIZE 10
typedef struct device_mailbox
{
	dev_msg_t *msg_ring[DEVICE_MAILBOX_SIZE];
	int write, read;

}dev_mailbox_t, WS_dev_mailbox_t;






#endif