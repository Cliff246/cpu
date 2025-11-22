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

	dev_mailbox_t *inbox;
	dev_mailbox_t *outbox;

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
}device_t;

typedef device_type_ptr_t (*device_init)(device_t *device, emuconfig_dev_settings_t *settings);
typedef void (*device_step)(device_t *device);
typedef dev_msg_t *(*device_read)(device_t *dev);
typedef void     (*device_send)(device_t *dev, dev_msg_t *msg);

typedef struct device_class
{
	device_init init;
	device_step update;
	device_read read;
	device_send send;

}device_class_t;

extern device_class_t device_vtable[DEVICES_TYPE_COUNT];

device_t *device_generate(emuconfig_dev_settings_t *settings );

void device_update(device_t *device);



#endif