#ifndef __WCPU_PART_SIGNAL_CD_TRANSFER_HEADER__
#define __WCPU_PART_SIGNAL_CD_TRANSFER_HEADER__

#include <stdint.h>

struct wcpu_part_signal_CD_TRANSFER
{
	uint64_t ct_base;
	uint64_t ct_len;
	uint64_t ins_base;
	uint64_t ins_len;
	uint64_t imm_base;
	uint64_t imm_len;
	uint64_t address;
};


#endif