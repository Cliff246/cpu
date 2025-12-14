#ifndef __WCPU_PART_DEPLOYER_HEADER__
#define __WCPU_PART_DEPLOYER_HEADER__


#include "wcpu_part.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct wcpu_part_deployer
{

}deployer_t;



part_ptr_t wcpu_deployer_generate(void);
void wcpu_deployer_update(part_t *part);

bool wcpu_deployer_import(part_t *part, part_signal_t *signal);
bool wcpu_deployer_export(part_t *part, part_signal_t **signal);

#endif