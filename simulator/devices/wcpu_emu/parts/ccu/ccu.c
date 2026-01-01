#include "ccu.h"
#include <assert.h>
part_ptr_t wcpu_ccu_generate(void)
{

	ccu_t *ccu = calloc(1, sizeof(ccu_t));
	assert(ccu);

	part_ptr_t pptr;
	pptr.ccu = ccu;
	return pptr;
}

void wcpu_ccu_update(part_t *part)
{

}

bool wcpu_ccu_import(part_t *part, part_signal_t *signal)
{
	return false;

}

bool wcpu_ccu_export(part_t *part, part_signal_t **signal)
{

	return false;

}

