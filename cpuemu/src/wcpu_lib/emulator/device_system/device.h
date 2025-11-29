#ifndef __DEVICE_HEADER__
#define __DEVICE_HEADER__

#include "device_type_ptr.h"
#include "device_message.h"
#include "device_mailbox.h"
#include "emulator_configure.h"


#include <stdint.h>
#include <stdlib.h>

typedef struct device_class device_class_t;




typedef struct device
{
	device_type_t type;

	dev_mailbox_t *mailbox;

	dev_id_t device_id;


	size_t address_range_start;
	size_t address_range_length;

	int error;
	//timers
	int timer_current;
	int timer_numerator;
	int timer_denominator;
	//device ptr
	device_type_ptr_t device;

	bool has_address;

}device_t;


//set up device
typedef device_type_ptr_t (*device_init_fn)(device_t *device, emuconfig_dev_settings_t *settings);
//complete execution
typedef void (*device_step_fn)(device_t *device);
//read from
typedef void (*device_read_fn)(device_t *dev, dev_msg_t *msg);
typedef dev_msg_t *(*device_send_fn)(device_t *dev);
typedef void (*device_print_fn)(device_t *dev);


typedef struct device_class
{
	device_init_fn init;
	device_step_fn update;
	device_read_fn read;
	device_send_fn send;
	device_print_fn print;
}device_class_t;


size_t get_device_address_start(device_t *device);
size_t get_device_address_length(device_t *device);
bool get_device_has_address(device_t *device);


//avoid modifying the mailbox through direct gets and sets
dev_mailbox_t *get_device_mailbox(device_t *dev);

//the vtable for are the device implementations
extern device_class_t device_vtable[DEVICES_TYPE_COUNT];


//generate devices
device_t *device_generate(emuconfig_dev_settings_t *settings );


//update devices
void device_update(device_t *device);


void device_read(device_t *device, dev_msg_t *msg);
dev_msg_t *device_send(device_t *device);

void device_print(device_t *device);


#endif