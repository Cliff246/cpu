#include "mattress.h"
#include <stdlib.h>



part_ptr_t wcpu_mattress_generate(void)
{
	mattress_t *mattress = calloc(1, sizeof(mattress_t));
	part_ptr_t pptr;
	pptr.mattress = mattress;
	return pptr;
}

void wcpu_mattress_update(part_t *mattress)
{

}

bool wcpu_mattress_import( part_t *part, part_signal_t *signal)
{
	return true;

}

bool wcpu_mattress_export( part_t *part, part_signal_t **signal)
{
	return false;
}