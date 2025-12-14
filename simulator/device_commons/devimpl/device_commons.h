#ifndef __DEVICE_COMMONS_HEADER__
#define __DEVICE_COMMONS_HEADER__

#include "device.h"
#include "device_message.h"
#include "device_mailbox.h"
#include "device_command.h"
#include "device_vtable.h"
#include "device_description.h"


size_t get_device_address_start(WS_dev_t *device);
size_t get_device_address_length(WS_dev_t *device);
bool get_device_has_address(WS_dev_t *device);


//avoid modifying the mailbox through direct gets and sets
dev_mailbox_t *get_device_mailbox(WS_dev_t *dev);

//the vtable for are the device implementations


//generate devices
//WS_dev_t *device_generate(emuconfig_dev_settings_t *settings );
WS_dev_t *device_init(device_command_t *cmd);


//update devices
void device_update(WS_dev_t *device);


bool device_read(WS_dev_t *device, WS_dev_msg_t *msg);
WS_dev_msg_t *device_send(WS_dev_t *device);

void device_print(WS_dev_t *device);
void device_cmd(WS_dev_t *device, device_command_t *cmd);

bool get_device_changed(WS_dev_t *device);

void set_device_changed(WS_dev_t *device);

extern char *WS_dev_type_str[];


device_command_t *device_command_generate(WS_dev_desc_t *desc);
void device_command_free(device_command_t *command);



void *device_init_config_setting(WS_dev_desc_t *type);
void device_free_config_setting(void *ptr, WS_dev_desc_t *type);

bool device_mailbox_put(dev_mailbox_t *mailbox, WS_dev_msg_t *msg);
bool device_mailbox_get(dev_mailbox_t *mailbox, WS_dev_msg_t **msg_ref);

dev_mailbox_t *device_mailbox_init(void);

void device_mailbox_print(dev_mailbox_t *mailbox);

WS_dev_msg_t *device_message_create(WS_dev_desc_t *src_type, WS_dev_id_t src_id, WS_dev_id_t dest_id, WS_dev_msg_type_t type, uint64_t address, int64_t value);


void device_message_respond(WS_dev_msg_t *msg, int64_t value);

uint64_t get_device_message_address(WS_dev_msg_t *msg);


WS_dev_msg_type_t get_device_message_type(WS_dev_msg_t *msg);
bool get_device_message_has_dst(WS_dev_msg_t *msg);


WS_dev_id_t get_device_message_src_id(WS_dev_msg_t *msg);
WS_dev_id_t get_device_message_dst_id(WS_dev_msg_t *msg);

void print_device_message(WS_dev_msg_t *msg);

bool device_message_consume(WS_dev_msg_t **msg);
bool device_message_release(WS_dev_msg_t **msg);


#endif