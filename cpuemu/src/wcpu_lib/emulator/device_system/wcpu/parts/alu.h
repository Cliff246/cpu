#ifndef __WCPU_PART_ALU_HEADER__
#define __WCPU_PART_ALU_HEADER__


#include "wcpu_part_ptr.h"

typedef struct wcpu_alu_operation
{

}alu_op_t;


//big fatty shared alu's and fpu's for the mattress to send to
typedef struct wcpu_part_alu
{

}alu_t;


part_ptr_t wcpu_alu_generate(void);

void wcpu_alu_update(part_t *ptr);

#endif