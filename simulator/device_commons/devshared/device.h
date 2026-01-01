#ifndef __DEVICE_HEADER__
#define __DEVICE_HEADER__

#include "device_list.h"

#include "device_message.h"
#include "device_mailbox.h"

//#include "emulator_configure.h"


#include <stdint.h>
#include <stdlib.h>

typedef struct device_description WS_dev_desc_t;
//set up device



typedef struct wcpu_simulator_device
{
	WS_dev_desc_t *desc;
	WS_dev_mailbox_t *mailbox;
	WS_dev_id_t id;
	char *name;

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
	bool commit;

}device_t, WS_dev_t;






#endif