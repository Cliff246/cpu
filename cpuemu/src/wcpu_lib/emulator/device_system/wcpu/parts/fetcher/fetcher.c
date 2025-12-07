#include "fetcher.h"
#include "wcpu_part.h"
#include "wcpu_part_ptr.h"
#include "wcpu_part_signal_fetcher_command.h"
#include "wcpu_part_signal_lsu_entry.h"
#include "fetcher_controller.h"
#include "fetcher_interface.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
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

	fetcher->controller = wcpu_fetcher_controller_create();
	fetcher->interface = wcpu_fetcher_interface_create();

	part_ptr_t pptr;
	pptr.fetcher = fetcher;

	return pptr;
}

void wcpu_fetcher_update(part_t *part)
{
	assert(part != NULL && "fetcher part cannot be null");
	assert(part->type == WCPU_PART_FETCHER && "part type for import must be of type WCPU_PART_fetcher");
	fetcher_t *fetcher = part->ptr.fetcher;

	//update all the ports with update
	wcpu_fetcher_controller_update(fetcher->controller, fetcher->interface);
	//clean all the set addresses this frame, should have been sent to ports
	wcpu_fetcher_interface_clean_ready(fetcher->interface);



}

bool wcpu_fetcher_import( part_t *part, part_signal_t *signal)
{
	assert(part != NULL && "part cannot be null");
	assert(part->type == WCPU_PART_FETCHER && "part type for import must be of type WCPU_PART_fetcher");
	fetcher_t *fetcher = part->ptr.fetcher;



	if(signal == NULL)
		return true;

	//send back into queue
	if(fetcher->backlog == true)
		return false;


	printf("import fetcher %d\n", signal->signal_type);






	//TEAR THIS OUT
	if(signal->signal_type == PART_SIGNAL_TYPE_FETCHER_COMMAND)
	{

		_part_signal_FETCHER_COMMAND_t *fetch_command = signal->ptr.FETCHER_COMMAND;
		if(fetch_command->code_desc_swap)
		{
			fetcher->controller->state = FETCHER_CONTROLLER_STATE_LOAD_DESC;

		}
	}
	//strong assumption here
	if(signal->signal_type == PART_SIGNAL_TYPE_LSU)
	{
		_part_signal_LSU_t *lsu = signal->ptr.LSU;
		wcpu_fetcher_interface_mark_import(fetcher->interface, lsu->address, lsu->value);
	}

	part_signal_consume(&signal);


	return true;
}

bool wcpu_fetcher_export( part_t *part, part_signal_t **signal)
{
	assert(part->type == WCPU_PART_FETCHER && "part type for import must be of type WCPU_PART_fetcher");
	fetcher_t *fetcher = part->ptr.fetcher;
	part_signal_t *export_signal;


	bool has_export = wcpu_fetcher_interface_pop_export(fetcher->interface, &signal);
	if(has_export)
	{
		*signal = export_signal;

	}
	return has_export;


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