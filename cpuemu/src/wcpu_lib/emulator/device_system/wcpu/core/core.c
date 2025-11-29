#include "core.h"
#include "wcpu_part_signal_ptr.h"
#include "wcpu_part_signal_core_io.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

void wcpu_core_clear_io(core_t *core)
{
	assert(core != NULL && "core cannot be null");

	core->core_io.address = 0;
	core->core_io.issued = false;
	core->core_io.responded = false;
	core->core_io.type = CORE_IO_NONE;
	core->core_io.value = 0;
}

int parts_order[] = {
	CORE_PARTS_LIST(CORE_PART_ORDER)
};


#define WCPU_SIGNALS_ALLOC 1000

static core_t *wcpu_create_core(void)
{


	core_t *core =  calloc(1, sizeof(core_t));
	assert(core != NULL && "core malloc cannot fail");
	size_t current = 0;
	//current part type
	for(int i = 0; i < COUNT_UNIQUE_CORE_PARTS; ++i)
	{

		for(int pi = 0; pi < parts_order[i]; ++pi)
		{
			core->parts[current++] = wcpu_part_generate(i);
			core->locations[i][pi] = current;
		}




	}


	return core;
}


core_t *wcpu_core_generate(void)
{
	return wcpu_create_core();
}

void wcpu_core_update(core_t *core)
{


	//printf("update core\n");
	//deploy core messages
	wcpu_core_handle_messages(core);





	//import all of the messages by popping off your imports
	for(int ia = 0; ia < COUNT_CORE_PARTS; ++ia)
	{
		part_t *part = core->parts[ia];

		part_signal_t *signal;

		while(pop_signal_off_channel(&part->bus.import, &signal))
		{
			wcpu_part_import(part, signal);
		}
	}

	for(int ib =0; ib < COUNT_CORE_PARTS; ++ib)
	{
		part_t *part = core->parts[ib];

		wcpu_part_step(part);

	}

	for(int ic =0; ic < COUNT_CORE_PARTS; ++ic)
	{
		part_t *part = core->parts[ic];
		part_signal_t *signal;
		while(wcpu_part_export(part, &signal))
		{
			if(signal == NULL)
				continue;
			bool push_result = push_signal_onto_channel(&part->bus.export, signal);
			if(push_result == false)
			{
				continue;
			}
		}
	}

}

typedef void (*wcpu_signal_unique)(core_t *core, part_signal_t *signal);
//core signal table, allows for special signals to be sent with core. allowing for the cpu to function
typedef struct core_signal_handle
{
	wcpu_signal_unique fn;
	bool distrubutes;

}core_signal_handle_t;

//just some stubs of the input and core
void wcpu_core_input(core_t *core, part_signal_t *signal)
{
	assert(signal->signal_type == PART_SIGNAL_TYPE_CORE_INPUT && "signal type must be core input");
	if(core->core_io.issued == false)
	{
		_part_signal_CORE_INPUT_t coreinp = *signal->ptr.CORE_INPUT;
		core->core_io.issued = true;
		core->core_io.type = CORE_IO_READ;
		core->core_io.address = coreinp.addr;
		//something like this
		part_signal_consume(signal);
	}

}

void wcpu_core_output(core_t *core, part_signal_t *signal)
{
	assert(signal->signal_type == PART_SIGNAL_TYPE_CORE_OUTPUT && "signal type must be core output");
	if(core->core_io.issued == false)
	{
		_part_signal_CORE_OUTPUT_t coreout = *signal->ptr.CORE_OUTPUT;
		core->core_io.issued = true;
		core->core_io.type = CORE_IO_WRITE;
		core->core_io.address = coreout.addr;
		core->core_io.value = coreout.value;

		//something like this
		part_signal_consume(signal);
	}
}


//static handles for all signal types
static core_signal_handle_t signal_handles[] =
{
	[PART_SIGNAL_TYPE_LSU] = {.distrubutes = true, .fn = NULL},
	[PART_SIGNAL_TYPE_CORE_INPUT] = {.distrubutes = false, .fn = wcpu_core_input},
	[PART_SIGNAL_TYPE_CORE_OUTPUT] = {.distrubutes = false, .fn = wcpu_core_output},
	//todo
	[PART_SIGNAL_TYPE_CORE_MEM_RESPONSE] = {.distrubutes = true, .fn = NULL},
	//todo
	[PART_SIGNAL_TYPE_REG_TRANSFER] = {.distrubutes = true, .fn = NULL},

	//todo
	[PART_SIGNAL_TYPE_LEDGER_TRANSFER] = {false, NULL},
	//todo
	[PART_SIGNAL_TYPE_AGGREGATOR_COMMAND] = {false, NULL},
};


//all signals must be in the signal handle system
static_assert((sizeof(signal_handles)/sizeof(signal_handles[0])) == PART_SIGNAL_ENUM_COUNT);

void wcpu_core_handle_messages(core_t *core)
{
	assert(core != NULL && "core cannot be null");
	for(int i = 0; i < COUNT_CORE_PARTS; ++i)
	{
		part_t *part = core->parts[i];
		assert(part != NULL && "assert must always be correct");
		part_signal_t *signal;
		//god dammit
		while(pop_signal_off_channel(&part->bus.export, &signal))
 		{
			assert(signal->signal_type >= 0 && "signal cannot have a type less than 0");
			assert(signal->signal_type < PART_SIGNAL_ENUM_COUNT && "signal type out of range");
			assert(signal->dst_id >= 0 && signal->dst_id < COUNT_CORE_PARTS && "dst id is not valid ");



			//do a unique signal operation in the core
			if(signal_handles[signal->signal_type].fn != NULL)
			{
				signal_handles[signal->signal_type].fn(core, signal);
			}
			if(signal_handles[signal->signal_type].distrubutes)
			{
				bool push_result = push_signal_onto_channel(&core->parts[signal->dst_id]->bus.import, signal);
				assert(push_result == true && "push must always be true");

			}
			else
			{
				//TODO handle signal that does not distrubute
			}
		}
	}

	//dispatch core signals around core_io to lsu

	if(core->core_io.responded == true && core->core_io.issued == true)
	{

		_part_signal_CORE_MEM_RESPONSE_t *pscmr = calloc(1, sizeof(_part_signal_CORE_MEM_RESPONSE_t));
		//sets the values
		pscmr->address = core->core_io.address;
		pscmr->load_value = core->core_io.value;

		part_signal_content_ptr_t content;
		content.CORE_MEM_RESPONSE = pscmr;
		//this is really really stupid

		//this essentially takes the first possible lsu and assumes its the source... need to fix this
		int lsu_dev_id = core->locations[WCPU_PART_LSU][0];


		//this psig might be lost lol
		part_signal_t *psig = part_signal_create(PART_SIGNAL_TYPE_CORE_MEM_RESPONSE, -1, lsu_dev_id, content);
		bool push_result = push_signal_onto_channel(&core->parts[psig->dst_id]->bus.import, psig);
		assert(push_result == true && "push must always be true");
		//wcpu_core_clear_io(core);
		//releases this signal in the core

		printf("%d", core->core_io.value);

		part_signal_release(psig);

	}


}
