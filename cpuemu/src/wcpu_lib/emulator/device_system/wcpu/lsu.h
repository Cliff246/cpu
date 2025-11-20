#ifndef __LSU_HEADER__
#define __LSU_HEADER__

#include <stdint.h>
#include "wcpu_part.h"


#define MAX_LSU_ENTRIES 128

typedef struct lsu_entry
{
	int64_t data;
	int64_t address;
	int epoch;

}lsu_entry_t;


typedef struct lsu
{
	lsu_entry_t entries[MAX_LSU_ENTRIES];




}lsu_t;

part_ptr_t wcpu_lsu_generate(void);
void wcpu_lsu_update(part_ptr_t lsu);

uint64_t load(uint64_t address);

void store(uint64_t address, int64_t value);

uint64_t address(uint64_t addr);


#endif