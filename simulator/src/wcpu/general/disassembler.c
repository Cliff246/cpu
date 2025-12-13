#include "disassembler.h"
//#include "core.h"
#include "code_decoder.h"
//#include "coreutils.h"
#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>

/*

int create_operation_range(operation_t **ops, int inst_start, int inst_len, int imm_start, int imm_len)
{

	int insts = inst_len * 2;
	operation_t *opptr = calloc(insts, sizeof(operation_t));

	int immoffset = 0;
	for(int i = 0; i < insts; ++i)
	{
		int32_t inst_bin = get_inst_at_pc_address(i);
		int64_t imm_bin = load(immoffset + imm_start);
		opptr[i] = fill_operation(inst_bin, imm_bin);
		decode_operation(&opptr[i]);
		//print_inst(&opptr[i].inst.inst);
		if(opptr[i].imm.type == IMM_TRUE)
		{
			immoffset++;
		}
	}

	*ops = opptr;
	return insts;
}


code_block_t *create_block_from_current_description(cpu_t *cpu, int range_start, int range_end)
{
	code_block_t *block = calloc(1, sizeof(code_block_t));



	reg_file_t curfile =  get_current_file(cpu);
	code_desc_t cd = curfile.desc;
	int size = create_operation_range(&block->ops, cd.pc_ptr, cd.pc_len, cd.ipc_ptr, cd.ipc_len);
	block->op_count = size;
	return block;
}
*/

