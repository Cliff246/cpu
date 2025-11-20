#ifndef __LSU_HEADER__
#define __LSU_HEADER__

#include <stdint.h>

#define MAX_LSU_ENTRIES 128

typedef struct lsu_entry
{
	int64_t data;
	int64_t address;
	int epoch;

}lsu_entry_t;


typedef struct wcpu_lsu
{
	lsu_entry_t entries[MAX_LSU_ENTRIES];




}lsu_t;

uint64_t load(uint64_t address);

void store(uint64_t address, int64_t value);

uint64_t address(uint64_t addr);


#endif