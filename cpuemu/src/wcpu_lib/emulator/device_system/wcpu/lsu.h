#ifndef __LSU_HEADER__
#define __LSU_HEADER__

#include <stdint.h>

typedef struct wcpu_lsu
{
	int incoming[10];
	int outgoint[10];
}lsu_t;

uint64_t load(uint64_t address);

void store(uint64_t address, int64_t value);

uint64_t address(uint64_t addr);


#endif