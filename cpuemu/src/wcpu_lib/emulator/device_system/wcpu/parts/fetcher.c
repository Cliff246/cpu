#include "fetcher.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

uint32_t get_inst_at_pc_address(uint64_t address)
{



	uint64_t dest = (address / 2); //get_pc_offset();
	if(address % 2 == 0)
	{
		return 0;//(uint32_t)((uint64_t)(MEMLD(dest) >> 32) & 0xffffffff);

	}
	else
	{
		return 0;//(uint32_t)((MEMLD(dest) & 0xffffffff));
	}
}

part_ptr_t wcpu_fetcher_generate(void)
{
	fetcher_t *fetcher = calloc(1, sizeof(fetcher_t));

	part_ptr_t pptr;
	pptr.fetcher = fetcher;

	return pptr;
}

void wcpu_fetcher_update(part_t *fetcher)
{
	assert(fetcher == NULL && "fetcher part cannot be null");
}