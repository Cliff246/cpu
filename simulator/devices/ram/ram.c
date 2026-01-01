#include "device.h"
#include "device_list.h"
#include "ram.h"
#include "ram_device_config_setting.h"
#include "device_commons.h"
#include "device_description.h"
#include "device_vtable.h"
#include "hashmap.h"
#include "device_command_impl.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <assert.h>



static const WS_dev_vtable_t vtable =
{
	.init = device_ram_init,
	.print = device_ram_print,
	.read = device_ram_read,
	.send = device_ram_send,
	.update = device_ram_update,
	.cmd_commit = device_ram_commit,
};

static WS_dev_desc_t ram_desc =
{
	.id = 0,
	.version = 0,

	.dev_name = "ram",
	.dev_typeclass = "mmio",
	.dl_name = "sim_dev_ram",
	.vtable = &vtable,
	.extra = NULL,

};







const WS_dev_desc_t *WS_get_dev_desc(void)
{
	static bool initialized = false;

	if(initialized == false)
	{
		p_hashtable_t hashtable = new_hash_table(DEVICE_RAM_CMD_OPTIONS_COUNT + WS_DEV_FLAG_DEFAULT_COUNT, WS_cmd_producer_free);

		for(int i = 0; i < DEVICE_RAM_CMD_OPTIONS_COUNT; ++i)
		{

			WS_dev_flag_producer_t *producer = WS_cmd_flag_producer_create(device_ram_producer_names[i], device_ram_producer_types[i], device_ram_producer_functions[i]);

			addto_hash_table(hashtable, device_ram_producer_names[i], producer);

		}

		//print_hash_table(hashtable);
		ram_desc.flag_table = hashtable;
		initialized = true;
	}


	return &ram_desc;
}





const static size_t ram_base_size = 1000;
const static size_t ram_base_start = 0;




void *device_ram_init(device_t *device)
{
	assert(device != NULL && "device must not be null");


	//dev_ram_config_setting_t *config = (dev_ram_config_setting_t *)cmd->setting;

	dev_ram_t *ram = create_ram(ram_base_size);

	//assert(config->settings[DEVICE_RAM_CONFIG_SETTING_ENABLE_FLAG_RESET] && "generate must force a reset");
	//cmd_ram(ram, cmd);
	align_ram(device, ram);
	device->has_address = true;

	return ram;
}



dev_ram_t *create_ram(int64_t length)
{
	dev_ram_t *ptr = (dev_ram_t *)calloc(1, sizeof(dev_ram_t));
	assert(ptr != NULL);

	int64_t *buffer = (int64_t *)calloc(length, sizeof(int64_t));
	assert(buffer != NULL);
	ptr->length = length;
	ptr->content = buffer;
	ptr->current_msg = NULL;
	return ptr;
}

//TODO make this make sense
void update_ram(dev_ram_t *ram, uint64_t length)
{

	assert(ram != NULL && "ram cannot be null");
	//get rid of the old
	free(ram->content);
	int64_t *buffer = (int64_t *)calloc(length, sizeof(int64_t));
	assert(buffer != NULL);
	ram->length = length;
	ram->content = buffer;



	return;
}


/*
static void cmd_ram(dev_ram_t *ram,  device_command_t *cmd)
{
	assert(ram && cmd);
	dev_ram_config_setting_t *config = (dev_ram_config_setting_t *)cmd->setting;

	assert(config->settings[DEVICE_RAM_CONFIG_SETTING_ENABLE_FLAG_RESET]);
	//big flag, no need to have a data type, your either reseting or not
	if(config->settings[DEVICE_RAM_CONFIG_SETTING_ENABLE_FLAG_RESET])
	{

		//set size
		if(config->settings[DEVICE_RAM_CONFIG_SETTING_ENABLE_FLAG_FILENAME])
		{
			assert(config->settings[DEVICE_RAM_CONFIG_SETTING_ENABLE_FLAG_SIZE] == false && "ram size cannot be a set size and load file");

			load_file(ram, config->filename);
			ram->local_address_size = ram->length;

		}
		else
		{
			//force reset to zero always... should be done via calloc but better safe then sorry
			if(config->settings[DEVICE_RAM_CONFIG_SETTING_ENABLE_FLAG_SIZE])
			{

				update_ram(ram, config->size);
				ram->local_address_size = ram->length;

			}
			else
			{
				//rest to base
				update_ram(ram, ram_base_size);

				ram->local_address_size = ram->length;
			}

			for(int i = 0; i < ram->length; ++i)
			{
				ram->content[i] = 0;
			}
		}

		//set start
		if(config->settings[DEVICE_RAM_CONFIG_SETTING_ENABLE_FLAG_START])
		{
			ram->local_address_start = config->start;

		}
		else
		{
			ram->local_address_start = ram_base_start;

		}

		//has to set changed
		ram->changed = true;
	}

	//set if we print content
	if(config->settings[DEVICE_RAM_CONFIG_SETTING_ENABLE_FLAG_PRINT_CONTENT])
	{
		//use the print content data
		ram->print_content_flag = config->print_content;
	}

}
*/

void align_ram(device_t *device, dev_ram_t *ram)
{
	assert(device != NULL && "device must not be null");
	assert(ram);
	device->address_range_length = ram->local_address_size;
	device->address_range_start = ram->local_address_start;
	ram->changed = false;
	device->has_address = true;
	WS_set_device_changed(device);

}


void device_ram_update(device_t *device)
{
	assert(device != NULL && "device must not be null");

	//essentially skiped
}

bool device_ram_read(WS_dev_t *dev, WS_dev_msg_t *msg)
{
	assert(dev != NULL && "device must not be null");

	dev_ram_t *ram = (dev_ram_t *)dev->ptr;

//this is flawed, can overwrite and unset current memoey operation, added current_msg == NULL check
	if(msg == NULL && ram->current_msg == NULL)
	{
		ram->current_msg = NULL;
		ram->has_msg = false;
	}
	else
	{
		assert(ram->has_msg == false && "ram cannot over write a inflight message");
		ram->current_msg = msg;
		dev_msg_type_t type = WS_get_device_message_type(msg);
		if(type == DEVMSG_WRITE)
		{
			printf("ram write %d:%d\n", msg->address, msg->value);
			write_ram(ram, msg->address, msg->value);
			ram->has_msg = false;
		}
		else if(type == DEVMSG_READ_SEND)
		{
			WS_device_message_consume(&ram->current_msg );
			ram->has_msg = true;
			return true;
		}
		else
		{
			assert(0 && "invalid message type");
		}
	}
	return false;
}
bool device_ram_send(WS_dev_t *dev, WS_dev_msg_t **msg)
{

	assert(dev != NULL && "device must not be null");

	dev_ram_t *ram = (dev_ram_t *)dev->ptr;
	if(ram->has_msg == false )
	{
		*msg = NULL;
		return false;
	}
	else
	{
		dev_msg_t *current = ram->current_msg;

		//get the read address
		uint64_t read_address = WS_get_device_message_address(current);
		//this realigns the correct read with the start of addressing
		uint64_t correct_read = read_address -  dev->address_range_start;
		assert(correct_read < ram->length && "correct read must not be invalid");

		int64_t respond = read_ram(ram, correct_read);

		//the response a bit brittle
		WS_device_message_respond(current, respond);
		ram->has_msg = false;
		ram->current_msg = NULL;


		*msg = current;
		return true;
	}

}

int64_t read_ram(dev_ram_t *ram, uint64_t address)
{
	assert(ram);
	assert(ram->length > address && address >= 0 && "read out of range");

	return ram->content[address];
}

void write_ram(dev_ram_t *ram, uint64_t address, int64_t data)
{
	assert(ram);
	assert(ram->length > address && address >= 0 && "write out of range");
	ram->content[address] = data;
}

void device_ram_print(device_t *dev)
{
	assert(dev != NULL && "device must not be null");

	dev_ram_t *ram = (dev_ram_t *)dev->ptr;
	if(ram->has_msg)
	{
		printf("\tram msg: ");
		WS_print_device_message(ram->current_msg);
		printf("\n");
	}
	if(ram->print_content_flag)
	{
		for(int i = 0; i < ram->length; ++i)
		{
			printf("[%d] = [%lld]\n", i, read_ram(ram, i));
		}
	}

}





void device_ram_commit(WS_dev_t *dev)
{
	dev_ram_t *ram = (dev_ram_t *)dev->ptr;

	align_ram(dev, ram);
}