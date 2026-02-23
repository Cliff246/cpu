#ifndef __DEBUG__
#define __DEBUG__

#include "flags.h"

typedef struct cpu_breakpoint
{
	int index;
}wcpu_breakpoint_t;

typedef struct wcpu_internal
{

}wcpu_internal_t;


#define DEBUG_MODE 0


const char *get_alu_inst_str(int i);
const char *get_jmp_inst_str(int i);
const char *get_mem_inst_str(int i);
const char *get_sys_inst_str(int i);



wcpu_internal_t basic_debug_getter(char *key);

void basic_debug_setter(char *key, int value);






#endif
