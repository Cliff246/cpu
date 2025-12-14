#ifndef __DEVICE_COMMONS_HEADER__
#define __DEVICE_COMMONS_HEADER__

#include "device.h"
#include "device_message.h"
#include "device_mailbox.h"
#include "device_command.h"


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


device_command_t *device_command_generate(device_type_t type);
void device_command_free(device_command_t *command);



void *device_init_config_setting(device_type_t type);
void device_free_config_setting(void *ptr, device_type_t type);

bool device_mailbox_put(dev_mailbox_t *mailbox, dev_msg_t *msg);
bool device_mailbox_get(dev_mailbox_t *mailbox, dev_msg_t **msg_ref);

dev_mailbox_t *device_mailbox_init(void);

void device_mailbox_print(dev_mailbox_t *mailbox);

dev_msg_t *device_message_create(WS_dev_desc_t *src_type, dev_id_t src_id, dev_id_t dest_id, dev_msg_type_t type, uint64_t address, int64_t value);


void device_message_respond(dev_msg_t *msg, int64_t value);

uint64_t get_device_message_address(dev_msg_t *msg);


dev_msg_type_t get_device_message_type(dev_msg_t *msg);
bool get_device_message_has_dst(dev_msg_t *msg);


WS_dev_id_t get_device_message_src_id(dev_msg_t *msg);
WS_dev_id_t get_device_message_dst_id(dev_msg_t *msg);

void print_device_message(dev_msg_t *msg);

bool device_message_consume(dev_msg_t **msg);
bool device_message_release(dev_msg_t **msg);


#endif