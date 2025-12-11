
#include "flags.h"
#include "vm.h"
#include "vm_cpu.h"
#include "vm_jmp.h"
#include <stdlib.h>
#include <stdio.h>

/*
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
	/*
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

	int64_t closest = 0;
	if(rel_address  != 0)
	{
		closest = rel_address / CODE_DESC_STRIDE;

	}

	uint64_t relative = load(ct_addr + closest);
	//printf("c:%ld r:%ld\n", closest, relative);
	return relative;
}

//does binary search on table and adds to cache if missing

uint64_t find_immediate_from_rel_table(cpu_t *cpu, uint64_t address)
{
	uint64_t ct_address = 0;
	uint64_t ct_len = 0;
	uint64_t cd_ptr = 0;
	reg_file_t file = get_current_file(cpu);
	cd_ptr = file.desc.cd_ptr;
	ct_address = file.desc.ct_ptr;
	ct_len = file.desc.ct_len;


	//check in book marks
	uint64_t idx = bookmark_index(address, cd_ptr);

	if(cpu->bookmarks[idx].valid && cpu->bookmarks[idx].cd_addr == cd_ptr)
	{
		//printf("bookmarked\n");
		return get_from_bookmark(cpu, address, cd_ptr);
	}

	uint64_t closest = 0;
	///


	if(ct_len < 1)
	{
		closest = 0;

	}
	else
	{
		//does binary search on the table
		closest =  closest_rel(cpu, ct_address, ct_len, address);
	}
	uint64_t inst_addr = (address / CODE_DESC_STRIDE);
	uint64_t imm_addr = load(closest + ct_address);
	//printf("%d address\n", address);
	for(int i = 0; i < CODE_DESC_STRIDE; ++i)
	{
		uint32_t instruction = get_inst_at_pc_address(inst_addr);

		//printf("iaddr: %ld instruction: %x\n", inst_addr, instruction);
		if(address == inst_addr)
			break;
		if ((instruction & 0x1) > 0)
		{
			imm_addr++;
		}
		inst_addr++;

	}
	//printf("\ninst address: %llu with imm address:%llu\n\n", inst_addr, imm_addr );
	cpu->bookmarks[idx].inst_addr = inst_addr ;

	cpu->bookmarks[idx].imm_addr  = imm_addr;
	cpu->bookmarks[idx].cd_addr = cd_ptr;
	cpu->bookmarks[idx].valid = 1;
	//ADD TO BOOK MARKS
	return imm_addr;
}

void call_to_outside(cpu_t *cpu, uint64_t rel_addr, uint64_t cd_addr)
{
	code_desc_t old_frame = get_desc(cpu->mode);

	push_scd(cpu, old_frame);

	code_desc_t current_frame = get_desc_from_address(cpu, cd_addr);

	set_desc(cpu->mode, current_frame);

  	uint64_t imm = find_immediate_from_rel_table(cpu, rel_addr);
    set_pc(rel_addr);
    set_ipc(imm);
}

void ret_from_outside(cpu_t *cpu)
{
    code_desc_t frame = pop_scd(cpu);
    set_desc(cpu->mode, frame);
    set_pc(frame.pc);
    set_ipc(frame.ipc);
}


void jump_to(cpu_t *cpu, uint64_t address)
{
	uint64_t imm = find_immediate_from_rel_table(cpu, address);
	//print_regs();

	//printf("\njumpto pc=%d ipc=%d\n\n", address, imm );
	//jump to
	set_pc(address);
	set_ipc(imm);
	cpu->has_jumped = true;


}

void jump_call(cpu_t *cpu, uint64_t target, char immf)
{
	store(inc_sp(1), get_pc() + 1);
	store(inc_sp(1), get_ipc() + ((immf)? 1: 0)) ;
	store(inc_sp(1), get_sfp());
	//memory_print(components.mem, 1000, 1010);
	//printf("\n\nipc %d\n\n", get_ipc());

	//printf("sp = %d\n", get_sp());
	set_sfp(get_sp());
	//printf("CALL before sp=%lu sfp=%lu\n", get_sp(), get_sfp());

	jump_to(cpu, target);

}

void jump_return(cpu_t *cpu)
{
	set_sp(get_sfp());

    uint64_t sfp = load(dec_sp(1));   // last pushed
    uint64_t ipc = load(dec_sp(1));   // second pushed
    uint64_t pc  = load(dec_sp(1));
	//printf("RET after sp=%lu sfp=%lu\n", get_sp(), get_sfp());

	set_sfp(sfp);
	set_ipc(ipc);
	set_pc(pc);
	cpu->has_jumped = true;
	//printf("RET pop pc=%lu ipc=%lu sfp=%lu\n", pc, ipc, sfp);
}


void jump_submit(cpu_t *cpu, uint64_t subpath, int64_t lane1, int64_t lane2, int64_t lane3, char immf)
{
	//print_regs();
	LOG("subpath:%d l1:%d l2:%d l3:%d\n", subpath, lane1, lane2, lane3);
	switch(subpath)
	{
		case JP_JMP:
			//TODO revalute how this will be done
			jump_to(cpu, lane1 + lane2 + lane3);
			break;
		case JP_BEQ:
			if(lane1 == lane2)
			{
				jump_to(cpu, lane3);
			}
			break;
		case JP_BNE:
			if(lane1 != lane2)
			{
			 	jump_to(cpu, lane3);

			}
			break;
		case JP_BLT:
			if((int64_t)lane1 < (int64_t)lane2)
			{
				jump_to(cpu, lane3);
				//printf("branch less equals %d %d\n", lane1, lane2);

			}
			break;
		case JP_BLE:
			if((int64_t)lane1 <= (int64_t)lane2)
			{
				jump_to(cpu, lane3);
				//printf("branch less than equals %d %d\n", lane1, lane2);
			}
			break;

		case JP_CALL:
			{

				//TODO revaluate if lane1 + lane2 + lane3 should be done
				jump_call(cpu, lane1 + lane2 + lane3, immf);
			}
			break;
		case JP_RET:
			{
				jump_return(cpu);
			}
			break;
		case JP_BLEU:
			if((uint64_t)lane1 <= (uint64_t)lane2)
			{
				jump_to(cpu, lane3);
			}
			break;

		case JP_BLTU:
			if((uint64_t)lane1 < (uint64_t)lane2)
			{
				jump_to(cpu, lane3);
			}
			break;

		default:
			break;

	}
}
*/


void vm_cpu_path_jmp_init(vima_t *vm)
{

}

int64_t vm_cpu_path_jmp_exec(vima_t *vm, char subflag, int64_t lane1, int64_t lane2, int64_t lane3, bool swap)
{
	return 0;

}