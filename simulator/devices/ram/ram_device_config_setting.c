#include "device.h"
#include "device_list.h"
#include "device_description.h"
#include "device_command.h"
#include "ram.h"
#include "ram_device_config_setting.h"
#include "device_command_impl.h"

#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>


WS_dev_cmd_flag_apply_fn device_ram_producer_functions[DEVICE_RAM_CMD_OPTIONS_COUNT] =
{
	DEVICE_RAM_CMD_OPTIONS_LIST(DEVICE_RAM_CMD_PRODUCER_FUNC_ARRAY)
};

char *device_ram_producer_names[DEVICE_RAM_CMD_OPTIONS_COUNT] =
{
	DEVICE_RAM_CMD_OPTIONS_LIST(DEVICE_RAM_CMD_PRODUCER_NAME_ARRAY)
};

WS_dev_cmd_flag_type_t device_ram_producer_types[DEVICE_RAM_CMD_OPTIONS_COUNT] =
{
	DEVICE_RAM_CMD_OPTIONS_LIST(DEVICE_RAM_CMD_PRODUCER_TYPE_ARRAY)

};

static void fill_binary(dev_ram_t *ram, uint64_t *bin, size_t length)
{
	assert(ram && bin);
	for(size_t i = 0; i < length; ++i)
	{
		write_ram(ram, i, bin[i]);
	}
}

static size_t file_len(FILE *fp)
{
	assert(fp);

	size_t current = ftell(fp);

	fseek(fp, 0, SEEK_END);
	size_t address = ftell(fp);

	fseek(fp, current, SEEK_SET);
	return address;
}

//returns true on change to size and false on no updated size
static bool load_file(dev_ram_t *ram, const char *file_name)
{
	errno = 0;
	FILE *fp = fopen(file_name, "rb");
	if(fp == NULL || errno != 0)
	{
		printf("file: %s not openable\n", file_name);
		assert(0);
		exit(1);

	}
	size_t len = file_len(fp);
	char *bytes = (char *)calloc(len, sizeof(char));
	assert(bytes);
	fread(bytes, 8, len / 8, fp);

	assert(ram != NULL && "cannot load file into unknown ram");
	bool changed = false;
	if(ram->length < len)
	{
		//sketchy
		update_ram(ram, len);
		changed = true;
	}

	uint64_t *bin = (uint64_t *)bytes;

	fill_binary(ram, bin, len / sizeof(uint64_t));
	free(bytes);
	fclose(fp);
	return changed;
}



bool device_cmd_option_producer_FILENAME(WS_dev_t *device, WS_dev_cmd_flag_t *flag)
{
	dev_ram_t *ram = device->ptr;
	load_file(ram, flag->value->STRING);

	ram->local_address_size = ram->length;
	ram->changed = true;
	return true;
}

bool device_cmd_option_producer_SIZE(WS_dev_t *device, WS_dev_cmd_flag_t *flag)
{
	dev_ram_t *ram = device->ptr;

	update_ram(ram, flag->value->INT);

	return true;

}

bool device_cmd_option_producer_START(WS_dev_t *device, WS_dev_cmd_flag_t *flag)
{
	dev_ram_t *ram = device->ptr;

	ram->local_address_start = flag->value->INT;
	return true;

}

bool device_cmd_option_producer_PRINT_CONTENT(WS_dev_t *device, WS_dev_cmd_flag_t *flag)
{
	dev_ram_t *ram = device->ptr;

	ram->print_content_flag = flag->value->BOOL;

	return true;

}

bool device_cmd_option_producer_RESET(WS_dev_t *device, WS_dev_cmd_flag_t *flag)
{
	return true;

}
