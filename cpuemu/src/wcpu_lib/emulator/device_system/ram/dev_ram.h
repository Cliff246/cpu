#ifndef __RAM_HEADER__
#define __RAM_HEADER__

#include <stdint.h>
#include <errno.h>
#include "device_type_ptr.h"
#include "device.h"
#include "emulator_configure.h"


typedef struct device_ram
{
	int64_t length;
	int64_t *content;
}dev_ram_t;

dev_ram_t *create_memory(int64_t length);


device_type_ptr_t device_ram_generate(device_t *device,emuconfig_dev_settings_t *settings);
void device_ram_update(device_t *device);


void write_ram(dev_ram_t *ram, int64_t address, int64_t data);
int64_t read_ram(dev_ram_t *ram, int64_t address);



#endif
