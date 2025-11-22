#include "core.h"
#include <stdlib.h>

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

}