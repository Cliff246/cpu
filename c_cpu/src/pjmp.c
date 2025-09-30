
#include "pjmp.h"
#include "core.h"
#include "flags.h"
#include "coreutils.h"
#include <stdlib.h>
#include <stdio.h>


uint64_t bookmark_index(uint64_t address, uint64_t cd_ptr)
{
	uint64_t idx = (address ^ cd_ptr) & (BOOKMARKS_MAX - 1);
	return idx;
}

uint64_t get_from_bookmark(cpu_t *cpu, uint64_t rel_address, uint64_t cd_ptr)
{
    // hash on both rel_address and cd_ptr
    uint64_t idx = bookmark_index(rel_address, cd_ptr);

    if (cpu->bookmarks[idx].valid &&
        cpu->bookmarks[idx].inst_addr == rel_address &&
        cpu->bookmarks[idx].cd_addr   == cd_ptr)
    {
        return cpu->bookmarks[idx].imm_addr; // cache hit
    }
    else
    {
        return 0; // miss
    }
}


uint64_t closest_rel(cpu_t *cpu, uint64_t ct_addr, uint64_t ct_len, uint64_t rel_address)
{
	int64_t closest = 0;
	int64_t low = 0;
	int64_t high = (ct_len / 2) - 1;

	while(low <= high)
	{

		closest = low + (high - low) / 2;
		if(load(ct_addr + closest * 2) == rel_address)
			closest = closest;
		if(load(ct_addr + closest * 2) < rel_address)
			low = closest + 1;
		else
			high = closest - 1;
	}
	return closest;
}

//does binary search on table and adds to cache if missing

uint64_t find_immedate_from_rel_table(cpu_t *cpu, uint64_t address)
{
	uint64_t ct_address = 0;
	uint64_t ct_len = 0;
	uint64_t cd_ptr = 0;
	if(cpu->mode == KERNAL)
	{
		cd_ptr = cpu->k_cd.cd_ptr;
		ct_address = cpu->k_cd.ct_ptr;
		ct_len = cpu->k_cd.ct_len;
	}
	else
	{
		cd_ptr = cpu->u_cd.cd_ptr;
		ct_address = cpu->u_cd.ct_ptr;
		ct_len = cpu->u_cd.ct_len;
	}

	//check in book marks
	uint64_t idx = bookmark_index(address, cd_ptr);

	if(cpu->bookmarks[idx].valid && cpu->bookmarks[idx].cd_addr == cd_ptr)
	{
		printf("bookmarked\n");
		return get_from_bookmark(cpu, address, cd_ptr);
	}

	uint64_t closest = 0;
	///


	if(ct_len < 2)
	{
		closest = 0;
	}
	else
	{
		//does binary search on the table
		closest =  closest_rel(cpu, ct_address, ct_len, address);
	}
	uint64_t inst_addr = load((closest * 2) + ct_address);
	uint64_t imm_addr = load((closest * 2) + ct_address + 1);

	for(int i = 0; i < 64 && i < get_pc_len(); ++i)
	{
		uint32_t instruction = load(inst_addr);

		if(address == inst_addr)
			break;
		if ((instruction & 0x3) > 0)
		{
			imm_addr++;
		}
		inst_addr++;

	}
	printf("\nimmedate address: %llu with offset:%llu\n\n",imm_addr, imm_addr );
	cpu->bookmarks[idx].inst_addr = inst_addr ;

	cpu->bookmarks[idx].imm_addr  = imm_addr ;
	cpu->bookmarks[idx].cd_addr = cd_ptr;
	cpu->bookmarks[idx].valid = 1;
	//ADD TO BOOK MARKS
	return imm_addr;
}

void call_to_outside(cpu_t *cpu, uint64_t rel_addr, uint64_t cd_addr)
{
	cd_frame_t old_frame = get_frame(cpu->mode);

	push_scd(cpu, old_frame);

	cd_frame_t current_frame = get_frame_from_address(cpu, cd_addr);

	set_frame(cpu->mode, current_frame);

  	uint64_t imm = find_immedate_from_rel_table(cpu, rel_addr);
    set_pc(rel_addr);
    set_ipc(imm);
}

void ret_from_outside(cpu_t *cpu)
{
    cd_frame_t frame = pop_scd(cpu);
    set_frame(cpu->mode, frame);
    set_pc(frame.pc);
    set_ipc(frame.ipc);
}


void jump_to(cpu_t *cpu, uint64_t address)
{
	uint64_t imm = find_immedate_from_rel_table(cpu, address);

	//printf("jumpto pc=%d ipc=%d\n", address - 1, imm);
	//jump to
	set_pc(address);
	set_ipc(imm);
	

}



void jump_submit(cpu_t *cpu, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t imm, char immf)
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
