#include "wcpu_part.h"
#include "deployer.h"
#include <assert.h>


part_ptr_t wcpu_deployer_generate(void)
{
	deployer_t *deployer = calloc(1, sizeof(deployer_t));
	assert(deployer);
	part_ptr_t pptr;
	pptr.deployer = deployer;

	return pptr;
}
void wcpu_deployer_update(part_t *part)
{

}

bool wcpu_deployer_import(part_t *part, part_signal_t *signal)
{

}

bool wcpu_deployer_export(part_t *part, part_signal_t **signal)
{

}