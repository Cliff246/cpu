#ifndef __MEMORY__
#define __MEMORY__

#include <stdint.h>
#include <errno.h>

typedef struct memory
{
	int64_t length;
	int64_t *content;	
}memory_t;


memory_t *create_memory(int64_t length);

int64_t memory_read(memory_t *ptr, int64_t address);
void memory_write(memory_t *ptr, int64_t address, int64_t value);


#endif
