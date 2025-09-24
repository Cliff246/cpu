#include "core.h"
#include "memory.h"
#include "flags.h"
#include "stdio.h"

typedef enum
{
	ZERO = 0,
	X1 = 1,
	X2 = 2,
	X3 = 3,
	X4 = 4,
	X5 = 5,
	X6 = 6,
	X7 = 7,
	X8 = 8,
	X9 = 9,
	X10 = 10,
	X11 = 11,
	X12 = 12,
	X13 = 13,
	X14 = 14,
	X15 = 15,
	X16 = 16,
	X17 = 17,
	X18 = 18,
	X19 = 19,
	X20 = 20,
	X21 = 21,
	X22 = 22,
	X23 = 23,
	X24 = 24,
	X25 = 25,
	X26 = 26,
	X27 = 27,
	X28 = 28,
	X29 = 29,
	X30 = 30,
	X31 = 31,

}cpu_regs;

#define ENC(p, sp, rd, rs1, rs2, aux, immf) encode(p, sp, rd, rs1, rs2, aux, immf)

#define AUX 0
#define SETMEM(i) components.mem->content[ i ]
#define INCMEM components.mem->content[ inc++]

#define PALU 0
#define PMEM 1
#define PJMP 2
#define PSYS 3

#define ALU_(msg) PALU, ALU_ ## msg
#define ENCALU(msg, rd, rs1, rs2, fl) encode(ALU_(msg), rd, rs1, rs2, AUX, fl); imms += 1
#define ENCSYS(msg) encode(PATH_SYS, msg, 0, 0, 0, 0, 0);
#define ENCJMP(msg, rs1, rs2, imm) encode(PATH_JMP, msg, 0, rs1, rs2, 0, imm); imms++;

void fill_binary(uint64_t *bin, int len)
{
	int32_t inc = 0;
	for(int i = 0; i < len; ++i)
	{
		INCMEM = bin[i];
	}
}

int setup_program()
{
	const int bin_size = 255;
	const int inst_ary_size = 255;
	uint64_t bin[bin_size] = {
		0
	};
	int table_info[inst_ary_size / 128 + 1 * 2] = {
		0
	};
	uint32_t insts[inst_ary_size] = {0};
	uint64_t imm_ary[inst_ary_size] = {1, 1,1,1,1,1000,9+3};

	int32_t inc = 0;
	int32_t imms = 0;
	//insts[inc++] = ENCSYS(SYS_SET_CD_PTR);
	insts[inc++] = ENCALU(ADD, X21, ZERO, ZERO, 1);
	insts[inc++] = ENCALU(ADD, ZERO, ZERO, ZERO, 1);
	insts[inc++] = ENCALU(ADD, ZERO, ZERO, ZERO, 1);
	insts[inc++] = ENCALU(ADD, ZERO, ZERO, ZERO, 1);
	insts[inc++] = ENCALU(ADD, X20, X21, X20, 1);
	insts[inc++] = ENCALU(ADD, ZERO, ZERO, ZERO, 1);
	insts[inc++] = ENCJMP(JP_JMP, ZERO, ZERO, 1);

	int table_size = ((inc / 128) + 1) * 2;
	int immediate_jump_table[inst_ary_size] = {0};
	int current_count = 0;
	int immediate_jump_table_index = 0;
	for(int inst_iter = 0; inst_iter < inc; inst_iter++)
	{
		if(inst_iter % 128 == 0)
		{
			immediate_jump_table[immediate_jump_table_index++] = current_count;
			current_count = 0;
		}
		if((insts[inst_iter] & 3) > 0)
		{
			current_count++;
		}

	}


	int offset = 0;
	//table
	bin[offset = 0] = 6;
	bin[offset = 1] = table_size;
	//inst ptr
	bin[offset = 2] = table_size + 6;
	bin[offset = 3] = inc;
	//imm ptr
	bin[offset = 4] = inc + table_size + 6;
	bin[offset = 5] = imms;
	offset++;
	for(int table = 0; table < table_size && table < immediate_jump_table_index; table++, offset+=2)
	{
		bin[offset] = table_size + 6 + (128 * table);
		bin[offset + 1] = inc + table_size + 6 + immediate_jump_table[table];

	}
	for(int c = 0; c < inc; ++c)
	{
		bin[offset++] = insts[c];
	}
	for(int i = 0; i < imms; ++i)
	{
		bin[offset++] = imm_ary[i];
	}
	for(int h = 0; h < offset; h++)
	{
		printf("%4d 0x%.8x %d\n", h, bin[h], bin[h]);
	}


	fill_binary(bin, bin_size);
}



int main(int argc, char *argv[])
{

	init_components();
	setup_program();
	startup_cpu();

	for(int i = 0; i < (100 * 10) + 5; ++i)
	{

		step_cpu();
	}
	print_regs();
}
