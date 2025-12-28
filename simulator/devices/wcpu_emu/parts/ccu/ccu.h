#ifndef __WCPU_PART_CCU_HEADER__
#define __WCPU_PART_CCU_HEADER__
#include "wcpu_part_signal.h"
#include "wcpu_part_ptr.h"
#include "wcpu_isa.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct wcpu_ccu_code_description
{
	uint64_t buffer;
	uint64_t address;
}ccu_code_description_t;

typedef struct wcpu_part_ccu
{
	ccu_code_description_t cd;

}ccu_t;


part_ptr_t wcpu_ccu_generate(void);
void wcpu_ccu_update(part_t *part);
bool wcpu_ccu_import(part_t *part, part_signal_t *signal);
bool wcpu_ccu_export(part_t *part, part_signal_t **signal);


#endif