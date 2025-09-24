
#include "pjmp.h"
#include "core.h"
#include "flags.h"
#include <stdlib.h>
#include <stdio.h>

uint64_t get_from_bookmark(cpu_t *cpu, uint64_t address)
{
	uint64_t idx = address & (BOOKMARKS_MAX - 1); // fast mask
	if (cpu->bookmarks[idx].valid && cpu->bookmarks[idx].inst_addr == address) {
    	return  cpu->bookmarks[idx].imm_addr; // cache hit
	}
	else
	{
		return 0;
	}
}


//does binary search on table and adds to cache if missing

uint64_t find_immedate_from_table(cpu_t *cpu, uint64_t address)
{
	uint64_t cd_address = 0;
	uint64_t cd_len = 0;
	if(cpu->mode == KERNAL)
	{
		cd_address = cpu->k_cd_ptr;
		cd_len = cpu->k_cd_len;
	}
	else
	{
		cd_address = cpu->u_cd_ptr;
		cd_len = cpu->u_cd_len;
	}

	//check in book marks
	uint64_t idx = address & (BOOKMARKS_MAX - 1); // fast mask

	if(cpu->bookmarks[idx].valid)
	{
		printf("bookmarked\n");
		return get_from_bookmark(cpu, address);
	}

	uint64_t closest = 0;
	///




	if(cd_len < 2)
	{
		closest = 0;
	}
	else
	{
		int64_t low = 0;
		int64_t high = (cd_len / 2) - 1;
		while(low <= high)
		{
			closest = low + (high - low) / 2;
			if(load(cd_address + closest * 2) == address)
				closest = closest;

			if(load(cd_address + closest * 2) < address)
				low = closest + 1;
			else
				high = closest - 1;
		}

	}
	uint64_t inst_addr = load(closest + cd_address);
	uint64_t imm_addr = load((closest * 2) + cd_address + 1);

	for(int i = 0; i < 64; ++i)
	{
		uint32_t instruction = load(inst_addr);
		printf("imm inst address %d %d %d\n", imm_addr, inst_addr, address);

		if(address == inst_addr)
			break;
		if ((instruction & 0x3) > 0)
		{
			printf("%d\n", instruction);
			imm_addr++;
		}
		inst_addr++;

	}
	cpu->bookmarks[idx].inst_addr = inst_addr;
	cpu->bookmarks[idx].imm_addr  = imm_addr;
	cpu->bookmarks[idx].valid     = 1;
	//ADD TO BOOK MARKS
	//bookmarks[inst_addr] = imm_addr
	return imm_addr;
}
void jump_to(cpu_t *cpu, uint64_t address)
{
	uint64_t imm = find_immedate_from_table(cpu, address);

	//jump to
	printf("jumpto pc=%d ipc=%d\n", address, imm);
	cpu->imm_pc = imm;
	cpu->inst_pc = address;


}

void jump_submit(cpu_t *cpu, u_int64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t imm, char immf)
{
	switch(subpath)
	{
		case JP_JMP:
			jump_to(cpu, rs1 + rs2 + imm);
			break;
		case JP_BEQ:
			if(rs1 == rs2)
				jump_to(cpu, imm);
			else
			break;
		case JP_BNE:
			break;
	}
}