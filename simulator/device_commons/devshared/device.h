#ifndef __DEVICE_HEADER__
#define __DEVICE_HEADER__

#include "device_flags.h"
#include "device_list.h"
#include "device_message.h"
#include "device_mailbox.h"
#include "device_command.h"
//#include "emulator_configure.h"


#include <stdint.h>
#include <stdlib.h>

typedef struct device_class device_class_t;
typedef struct device device_t;
//set up device
typedef void (*device_init_fn)(device_t *device, device_command_t *cmd);
//complete execution
typedef void (*device_step_fn)(device_t *device);
//read from
typedef bool (*device_read_fn)(device_t *dev, dev_msg_t *msg);
typedef dev_msg_t *(*device_send_fn)(device_t *dev);
typedef void (*device_print_fn)(device_t *dev);
typedef void (*device_cmd_fn)(device_t *dev, device_command_t *cmd);

typedef struct device_class
{
	device_init_fn init;
	device_step_fn update;
	device_read_fn read;
	device_send_fn send;
	device_print_fn print;
	device_cmd_fn cmd;
}device_class_t;

typedef struct device
{
	device_type_t type;
	device_class_t class;
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
	void *ptr;

	bool has_address;

	bool flags[DEVICE_FLAG_COUNT];

}device_t;






#endif