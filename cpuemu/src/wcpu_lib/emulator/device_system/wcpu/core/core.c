#include "core.h"
#include "wcpu_part_signal_ptr.h"
#include "wcpu_part_signal_core_io.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

int parts_order[] = {
	CORE_PARTS_LIST(CORE_PART_ORDER)
};


static core_t *wcpu_create_core(void)
{
	core_t *core =  calloc(1, sizeof(core_t));
	assert(core != NULL && "core malloc cannot fail");
	size_t current = 0;
	for(int i = 0; i < COUNT_CORE_PARTS; ++i)
	{

		for(int pi = 0; pi < parts_order[i]; ++pi)
		{
			core->parts[current++] = wcpu_part_generate(i);

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

	//do updates or whaterver


}

typedef void (*wcpu_signal_unique)(core_t *core, part_signal_t *signal);

typedef struct core_signal_handle
{
	wcpu_signal_unique fn;
	bool distrubutes;

}core_signal_handle_t;


void wcpu_core_input(core_t *core, part_signal_t *signal)
{

}

void wcpu_core_output(core_t *core, part_signal_t *signal)
{

}



static core_signal_handle_t signal_handles[] =
{
	[PART_SIGNAL_TYPE_LSU] = {.distrubutes = true, .fn = NULL},
	[PART_SIGNAL_TYPE_CORE_INPUT] = {.distrubutes = false, .fn = wcpu_core_input},
	[PART_SIGNAL_TYPE_CORE_OUTPUT] = {.distrubutes = false, .fn = wcpu_core_output}
};


//all signals must be in the signal handle system
static_assert((sizeof(signal_handles)/sizeof(signal_handles[0])) == PART_SIGNAL_ENUM_COUNT);

void wcpu_core_handle_messages(core_t *core)
{
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
}
