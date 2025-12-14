#ifndef __DEVICE_MAILBOX_IMPL_HEADER__
#define __DEVICE_MAILBOX_IMPL_HEADER__

bool device_mailbox_put(dev_mailbox_t *mailbox, dev_msg_t *msg);
bool device_mailbox_get(dev_mailbox_t *mailbox, dev_msg_t **msg_ref);

dev_mailbox_t *device_mailbox_init(void);

void device_mailbox_print(dev_mailbox_t *mailbox);

#endif