#include "cpumemory.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

memory_t *create_memory(int64_t length)
{
	memory_t *ptr = (memory_t *)malloc(sizeof(memory_t));
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

int64_t memory_read(memory_t *ptr, int64_t address)
{
	if(ptr->length > address && address >= 0)
	{
		return ptr->content[address];
	}
	else
	{
		errno = ERANGE;
		return 0;
	}
}
void memory_write(memory_t *ptr, int64_t address, int64_t value)
{
	if(ptr->length > address && address >= 0)
	{
		ptr->content[address] = value;
		return;
	}
	else
	{
		errno = ERANGE;
		return;
	}

}


void memory_print(memory_t *memory, uint64_t start, uint64_t stop)
{
	if(stop < start)
		return;
	uint64_t diff = stop - start;
	if(memory->length < stop)
		return;
	for(uint64_t i = start; i < stop; ++i)
	{
		printf("%4lld 0x%.8llx %llu\n", i, memory->content[i], memory->content[i]);
	}

}


