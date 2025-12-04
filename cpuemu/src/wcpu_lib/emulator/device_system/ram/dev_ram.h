#ifndef __RAM_HEADER__
#define __RAM_HEADER__


#include "device_list.h"
#include "device.h"
#include "emulator_configure.h"
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

typedef struct device_ram
{

	int64_t length;
	int64_t *content;

	dev_msg_t *current_msg;
	bool has_msg;

	//make this a boolean array index'd by a ram flag enum
	bool print_content_flag;

	uint64_t local_address_start;
	uint64_t local_address_size;
	bool changed;

}dev_ram_t;

static dev_ram_t *create_ram(int64_t length);
static void update_ram(dev_ram_t *ram, uint64_t length);
static void cmd_ram(dev_ram_t *ram,  device_command_t *cmd);
static void align_ram(device_t *device, dev_ram_t *ram);


device_type_ptr_t device_ram_generate(device_t *device,emuconfig_dev_settings_t *settings);
void device_ram_update(device_t *device);
void device_ram_read(device_t *dev, dev_msg_t *msg);
dev_msg_t *device_ram_send(device_t *dev);
void device_ram_print(device_t *dev);
void device_ram_cmd(device_t *device, device_command_t *cmd);

static void write_ram(dev_ram_t *ram, uint64_t address, int64_t data);
static int64_t read_ram(dev_ram_t *ram, uint64_t address);

#endif
