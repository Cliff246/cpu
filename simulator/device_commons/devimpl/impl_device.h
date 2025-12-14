#ifndef __DEVICE_IMPL_HEADER__
#define __DEVICE_IMPL_HEADER__

#include "device.h"


size_t get_device_address_start(device_t *device);
size_t get_device_address_length(device_t *device);
bool get_device_has_address(device_t *device);


//avoid modifying the mailbox through direct gets and sets
dev_mailbox_t *get_device_mailbox(device_t *dev);

//the vtable for are the device implementations


//generate devices
//device_t *device_generate(emuconfig_dev_settings_t *settings );


//update devices
void device_update(device_t *device);


bool device_read(device_t *device, dev_msg_t *msg);
dev_msg_t *device_send(device_t *device);

void device_print(device_t *device);
void device_cmd(device_t *device, device_command_t *cmd);

bool get_device_changed(device_t *device);

void set_device_changed(device_t *device);

extern char *device_type_str[];


#endif