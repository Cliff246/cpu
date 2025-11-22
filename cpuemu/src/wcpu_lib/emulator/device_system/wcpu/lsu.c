#include "lsu.h"
#include <stdlib.h>

part_ptr_t wcpu_lsu_generate(void)
{
	lsu_t *lsu = calloc(1, sizeof(lsu_t));
	part_ptr_t pptr;
	pptr.lsu = lsu;
	return pptr;
}

void wcpu_lsu_update(part_ptr_t lsu)
{

}
