#include "device.h"
#include "device_type_ptr.h"
#include "dev_ram.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <assert.h>


//this is not correct but it works for now, it doesnt work at offsets so if i try to load at 10000 and it starts at 9000 it wont load at 1000 it will load at 10000
device_type_ptr_t device_ram_generate(device_t *device, emuconfig_dev_settings_t *settings)
{
	device->address_range_start = 0;
	device->address_range_length = 20;

	dev_ram_t *ram = create_memory(20);
	device_type_ptr_t ptr;
	ptr.ram = ram;
	return ptr;
}



dev_ram_t *create_memory(int64_t length)
{
	dev_ram_t *ptr = (dev_ram_t *)malloc(sizeof(dev_ram_t));
	if(!ptr)
	{
		errno = ENOMEM;
		return NULL;
	}
	else
	{
		int64_t *buffer = (int64_t *)calloc(length, sizeof(int64_t));
		if(!buffer)
		{
			errno = ENOMEM;
			free(ptr);
			return NULL;
		}
		ptr->length = length;
		ptr->content = buffer;
		ptr->current_msg = NULL;
		return ptr;
	}
}


void device_ram_update(device_t *device)
{
	assert(device != NULL && "device must not be null");
	assert(device->type == DEVICE_RAM && "device must be of type ram");

	//essentially skiped







}


//read ram and get a message set it as active
void device_ram_read(device_t *dev, dev_msg_t *msg)
{
	assert(dev != NULL && "device must not be null");
	assert(dev->type == DEVICE_RAM && "device must be of type ram");

	dev_ram_t *ram = dev->device.ram;

	if(msg == NULL)
	{
		ram->current_msg = NULL;
		ram->has_msg = false;
	}
	else
	{
		assert(ram->has_msg == false && "ram cannot over write a inflight message");

		if(get_device_message_sendback(msg) == false)
		{
			write_ram(ram, msg->address, msg->value);
			device_message_release(msg);
			ram->current_msg = NULL;
			ram->has_msg = false;
		}
		else
		{
			device_message_consume(msg);
			ram->current_msg = msg;
			ram->has_msg = true;
		}





	}
	return;
}

dev_msg_t *device_ram_send(device_t *dev)
{

	assert(dev != NULL && "device must not be null");
	assert(dev->type == DEVICE_RAM && "device must be of type ram");

	dev_ram_t *ram = dev->device.ram;
	if(ram->has_msg == false)
	{
		return NULL;
	}
	else
	{
		dev_msg_t *current = device_message_consume(ram->current_msg);

		//get the read address
		uint64_t read_address = get_device_message_address(current);
		//this realigns the correct read with the start of addressing
		uint64_t correct_read = read_address -  dev->address_range_start;
		assert(correct_read < ram->length && "correct read must not be invalid");

		uint64_t respond = read_ram(ram, correct_read);

		//the response a bit brittle
		device_message_respond(current, respond);

		ram->has_msg = false;
		ram->current_msg = NULL;
		return current;
	}

}




int64_t read_ram(dev_ram_t *ram, int64_t address)
{
	assert(ram->length > address && address >= 0 && "read out of range");

	return ram->content[address];
}
void write_ram(dev_ram_t *ram, int64_t address, int64_t data)
{
	assert(ram->length > address && address >= 0 && "write out of range");
	ram->content[address] = data;


}



void device_ram_print(device_t *dev)
{
	assert(dev != NULL && "device must not be null");

	dev_ram_t *ram = dev->device.ram;


	for(int i = 0; i < ram->length; ++i)
	{
		printf("[%lld] = [%lld]\n", i, read_ram(ram, i));
	}
}
