#ifndef __DEBUG__
#define __DEBUG__

#include "flags.h"


#define DEBUG_MODE 0


const char *get_alu_inst_str(int i);
const char *get_jmp_inst_str(int i);
const char *get_mem_inst_str(int i);
const char *get_sys_inst_str(int i);

#endif
