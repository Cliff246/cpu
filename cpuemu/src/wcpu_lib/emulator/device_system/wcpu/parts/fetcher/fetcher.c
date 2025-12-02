#include "fetcher.h"
#include "wcpu_part.h"
#include "wcpu_part_ptr.h"
#include "wcpu_part_signal_fetcher_command.h"
#include "wcpu_part_signal_lsu_entry.h"
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

void wcpu_fetcher_update(part_t *part)
{
	assert(part != NULL && "fetcher part cannot be null");
	assert(part->type == WCPU_PART_FETCHER && "part type for import must be of type WCPU_PART_fetcher");
	fetcher_t *fetcher = part->ptr.fetcher;


	if(fetcher->load_code_descriptor == true)
	{
		if(fetcher->code_descriptor_count < 6)
		{
			fetcher->doload = true;
			fetcher->toload = fetcher->code_descriptor_address + fetcher->code_descriptor_count;
			fetcher->code_descriptor_count++;
		}
		else
		{
			fetcher->code_descriptor_count = 0;
			fetcher->finished_code_descriptor = true;
			fetcher->load_code_descriptor = false;
			fetcher->doload = false;
		}
	}

}

void wcpu_fetcher_import( part_t *part, part_signal_t *signal)
{
	assert(part != NULL && "part cannot be null");
	assert(part->type == WCPU_PART_FETCHER && "part type for import must be of type WCPU_PART_fetcher");
	fetcher_t *fetcher = part->ptr.fetcher;

	if(signal == NULL)
		return;
	printf("import fetcher %d\n", signal->signal_type);

	//TEAR THIS OUT
	if(signal->signal_type == PART_SIGNAL_TYPE_FETCHER_COMMAND)
	{

		_part_signal_FETCHER_COMMAND_t *fetch_command = signal->ptr.FETCHER_COMMAND;

		if(fetch_command->code_desc_swap == true)
		{
			fetcher->load_code_descriptor = true;
			fetcher->finished_code_descriptor = false;

			fetcher->code_descriptor_address = fetch_command->address;
			fetcher->code_descriptor_count = 0;
			for(int i = 0; i < 6; ++i)
			{
				fetcher->code_descriptor_chunk[i] = 0;

			}

		}
	}
	//strong assumption here
	if(signal->signal_type == PART_SIGNAL_TYPE_LSU)
	{
		_part_signal_LSU_t *lsu = signal->ptr.LSU;
		if(fetcher->load_code_descriptor == true)
		{
			//this is fucked
			fetcher->code_descriptor_chunk[lsu->address] = lsu->value;
		}
	}

	part_signal_consume(&signal);



}

bool wcpu_fetcher_export( part_t *part, part_signal_t **signal)
{
	assert(part->type == WCPU_PART_FETCHER && "part type for import must be of type WCPU_PART_fetcher");
	fetcher_t *fetcher = part->ptr.fetcher;


	if(fetcher->doload == true)
	{

		_part_signal_LSU_t *pull = calloc(1, sizeof(_part_signal_LSU_t));
		assert(pull != NULL);

		pull->loadstore = true;
		pull->address = fetcher->toload;

		part_signal_content_ptr_t pscp;
		pscp.LSU = pull;




		part_signal_t *sig = part_signal_create(PART_SIGNAL_TYPE_LSU, part->id, WCPU_PART_LSU, pscp);

		fetcher->doload = false;
		*signal = sig;

		return true;
	}
	else
	{
		return false;
	}



}