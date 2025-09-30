#ifndef __COREUTILS__
#define __COREUTILS__

#include "core.h"


void push_scd(cpu_t *cpu, cd_frame_t frame);
cd_frame_t pop_scd(cpu_t *cpu);

cd_frame_t get_frame_from_address(cpu_t *cpu, uint64_t address);








#endif