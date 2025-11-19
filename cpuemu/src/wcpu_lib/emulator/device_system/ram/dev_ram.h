#ifndef __RAM_HEADER__
#define __RAM_HEADER__

#include <stdint.h>
#include <errno.h>

typedef struct device_ram
{
	int64_t length;
	int64_t *content;
}dev_ram_t;


dev_ram_t create_ram(uint64_t size);


void write_ram(dev_ram_t *ram, int64_t address, int64_t data);
int64_t read_ram(dev_ram_t *ram, int64_t address);



#endif
