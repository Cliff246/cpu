#include "core.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int parts_order[] = {
	CORE_PARTS_LIST(CORE_PART_ORDER)
};


static core_t *wcpu_create_core(void)
{
	core_t *core =  calloc(1, sizeof(core_t));

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
	printf("update core\n");
	//deploy core messages
	wcpu_core_handle_messages(core);

	//do updates or whaterver


}



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
			assert(signal->signal_type != 0 && "signal cannot have a type of 0");
			assert(signal->dst_id >= 0 && signal->dst_id < COUNT_CORE_PARTS && "dst id is not valid ");
			bool push_result = push_signal_onto_channel(&core->parts[signal->dst_id]->bus.import, signal);
			assert(push_result == true && "push must always be true");
		}
	}
}
