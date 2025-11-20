#include "fetcher.h"
#include "stdint.h"

uint32_t get_inst_at_pc_address(uint64_t address)
{



	uint64_t dest = (address / 2) + get_pc_offset();
	if(address % 2 == 0)
	{
		return (uint32_t)((uint64_t)(MEMLD(dest) >> 32) & 0xffffffff);

	}
	else
	{
		return (uint32_t)((MEMLD(dest) & 0xffffffff));
	}
}

part_ptr_t wcpu_fetcher_generate(void)
{

}

void wcpu_fetcher_update(part_ptr_t fetcher)
{

}