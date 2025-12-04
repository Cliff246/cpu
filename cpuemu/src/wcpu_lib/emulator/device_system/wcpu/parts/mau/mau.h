#ifndef __WCPU_PART_MAU_HEADER__
#define __WCPU_PART_MAU_HEADER__


#include "wcpu_part_ptr.h"
#include "wcpu_part_signal.h"
#include <stdbool.h>

typedef struct wcpu_mau_operation
{

}mau_op_t;


//big fatty shared mau's and fpu's for the mattress to send to
typedef struct wcpu_part_mau
{

}mau_t;


part_ptr_t wcpu_mau_generate(void);

void wcpu_mau_update(part_t *ptr);
bool wcpu_mau_import( part_t *part, part_signal_t *signal);
bool wcpu_mau_export( part_t *part, part_signal_t **signal);

#endif