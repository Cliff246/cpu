#include "device.h"
#include "device_type_ptr.h"
#include "dev_ram.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>

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
		return ptr;
	}
}



int64_t read_ram(dev_ram_t *ram, int64_t address)
{

	if(ram->length > address && address >= 0)
	{
		return ram->content[address];
	}
	else
	{
		errno = ERANGE;
		return 0;
	}
}
void write_ram(dev_ram_t *ram, int64_t address, int64_t data)
{
	if(ram->length > address && address >= 0)
	{
		ram->content[address] = data;
		return;
	}
	else
	{
		errno = ERANGE;
		return;
	}

}



