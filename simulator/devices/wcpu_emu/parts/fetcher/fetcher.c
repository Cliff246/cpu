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
#include <stdbool.h>
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
	assert(fetcher);
	fetcher->controller = wcpu_fetcher_controller_create();
	fetcher->interface = wcpu_fetcher_interface_create();
	assert(fetcher->controller);
	assert(fetcher->interface);
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
	//wcpu_fetcher_interface_clean_ready(fetcher->interface);



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


	//printf("import fetcher %d\n", signal->signal_type);






	//TEAR THIS OUT
	if(signal->signal_type == PART_SIGNAL_TYPE_FETCHER_COMMAND)
	{

		_part_signal_FETCHER_COMMAND_t *fetch_command = signal->ptr.FETCHER_COMMAND;
		if(fetch_command->type == FETCHER_COMMAND_CD_SWAP)
		{
			//will free on the fetcher_controller... dangerous but it should be fine
			_fetcher_command_cd_swap_t cd_swap = fetch_command->cmd.cd_swap;
			//should
			fetcher_port_order_t *order = fetcher_port_code_description_order_create(cd_swap.address);

		}
		else if(fetch_command->type == FETCHER_COMMAND_CT_LOAD)
		{
			_fetcher_command_ct_load_t ct_load = fetch_command->cmd.ct_load;
		}
		else if(fetch_command->type == FETCHER_COMMAND_IMM_LOAD)
		{
			_fetcher_command_imm_load_t imm_load = fetch_command->cmd.imm_load;
		}
		else if(fetch_command->type == FETCHER_COMMAND_INS_LOAD)
		{
			_fetcher_command_ins_load_t ins_load = fetch_command->cmd.ins_load;
		}
		else
		{
			assert(0 && "fetch command not implemented");
		}
	}

	//strong assumption here
	if(signal->signal_type == PART_SIGNAL_TYPE_LSU)
	{
		_part_signal_LSU_t *lsu = signal->ptr.LSU;
		//printf("mark import %d %d\n", lsu->address, lsu->value);

		wcpu_fetcher_interface_mark_import(fetcher->interface, lsu->address, lsu->value);
	}

	part_signal_consume(&signal);


	return true;
}

bool wcpu_fetcher_export( part_t *part, part_signal_t **signal)
{
	assert(part);
	assert(part->type == WCPU_PART_FETCHER && "part type for import must be of type WCPU_PART_fetcher");
	fetcher_t *fetcher = part->ptr.fetcher;
	part_signal_t *export_signal;


	bool has_export = wcpu_fetcher_interface_pop_export(fetcher->interface, &export_signal);
	//printf("had export %d\n", has_export);
	if(has_export)
	{
		*signal = export_signal;
		return true;
	}

	wcpu_fetcher_interface_clear_iterators(fetcher->interface);

	bool import_to_queue = false;
	fetcher_entry_t entry = {0};
	import_to_queue = wcpu_fetcher_interface_use_import(fetcher->interface, &entry);

	if(import_to_queue)
	{
		_part_signal_LSU_t *pull = calloc(1, sizeof(_part_signal_LSU_t));
		assert(pull != NULL);

		pull->loadstore = true;
		pull->address = entry.address;
		//printf("import to queue used %d %d\n", import_to_queue, entry.address);

		part_signal_content_ptr_t pscp;
		pscp.LSU = pull;




		part_signal_t *sig = part_signal_create(PART_SIGNAL_TYPE_LSU, part->id, WCPU_PART_LSU, pscp);
		assert(sig);
		*signal = sig;

		return true;
	}
	else
	{
		return false;
	}



}