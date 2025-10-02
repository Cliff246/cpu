#ifndef __PJMP__
#define __PJMP__
#include <stdint.h>
#include "core.h"


uint64_t find_immedate_from_rel_table(cpu_t *cpu, uint64_t address);
void jump_to(cpu_t *cpu, uint64_t address);
void jump_submit(cpu_t *cpu, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t imm, char immf);

#endif
