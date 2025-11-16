#ifndef __PJMP__
#define __PJMP__
#include <stdint.h>
#include "core.h"


uint64_t find_immedate_from_rel_table(cpu_t *cpu, uint64_t address);
void jump_to(cpu_t *cpu, uint64_t address);
void jump_submit(cpu_t *cpu, uint64_t subpath, int64_t lane1, int64_t lane2, int64_t lane3, char immf);

#endif
