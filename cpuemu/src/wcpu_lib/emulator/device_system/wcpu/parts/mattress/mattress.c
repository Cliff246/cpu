#include "mattress.h"
#include <stdlib.h>

mattress_unit_t *wcpu_mattress_unit_generate(mattress_unit_type_t type)
{
	mattress_unit_t *unit = calloc(1, sizeof(mattress_unit_t));
	return unit;
}

part_ptr_t wcpu_mattress_generate(void)
{
	mattress_t *mattress = calloc(1, sizeof(mattress_t));
	part_ptr_t pptr;
	pptr.mattress = mattress;
	return pptr;
}