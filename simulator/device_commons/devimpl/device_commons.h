#ifndef __DEVICE_COMMONS_HEADER__
#define __DEVICE_COMMONS_HEADER__

#include "device.h"
#include "device_message.h"
#include "device_mailbox.h"
#include "device_command.h"
#include "device_vtable.h"
#include "device_description.h"


size_t WS_get_device_address_start(WS_dev_t *device);
size_t WS_get_device_address_length(WS_dev_t *device);
bool WS_get_device_has_address(WS_dev_t *device);


//avoid modifying the mailbox through direct gets and sets

//the vtable for are the device implementations


//generate devices
//WS_dev_t *device_generate(emuconfig_dev_settings_t *settings );
WS_dev_t *WS_device_init(WS_dev_desc_t *desc, device_command_t *cmd);


//update devices
void WS_device_update(WS_dev_t *device);


bool WS_device_read(WS_dev_t *device, WS_dev_msg_t *msg);
bool WS_device_send(WS_dev_t *device, WS_dev_msg_t **msg);


bool WS_get_device_changed(WS_dev_t *device);
void WS_set_device_changed(WS_dev_t *device);







bool WS_device_mailbox_put(WS_dev_t *dev, WS_dev_msg_t *msg);
bool WS_device_mailbox_pop(WS_dev_t *dev, WS_dev_msg_t **msg);

WS_dev_mailbox_t *WS_device_mailbox_init(void);

void WS_device_mailbox_print(WS_dev_t *dev);

WS_dev_msg_t *WS_device_message_create(WS_dev_desc_t *src_type, WS_dev_id_t src_id, WS_dev_id_t dest_id, WS_dev_msg_type_t type, uint64_t address, int64_t value);


void WS_device_message_respond(WS_dev_msg_t *msg, int64_t value);

uint64_t WS_get_device_message_address(WS_dev_msg_t *msg);


WS_dev_msg_type_t WS_get_device_message_type(WS_dev_msg_t *msg);
bool WS_get_device_message_has_dst(WS_dev_msg_t *msg);


WS_dev_id_t WS_get_device_message_src_id(WS_dev_msg_t *msg);
WS_dev_id_t WS_get_device_message_dst_id(WS_dev_msg_t *msg);
void WS_print_device_message(WS_dev_msg_t *msg);

bool WS_device_message_consume(WS_dev_msg_t **msg);
bool WS_device_message_release(WS_dev_msg_t **msg);


#endif