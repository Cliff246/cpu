#include "debug.h"
#include "flags.h"


#define STR(x) #x
#define CASE(name) case name :{ \
		return STR(name);}		\

#define ALU_CASE(post) CASE(ALU_ ## post)
#define JMP_CASE(post) CASE(JP_ ## post)
#define MEM_CASE(post) CASE(MEM_ ## post)
#define SYS_CASE(post) CASE(SYS_ ## post)


#define DEFAULT default: \
				return "NO OP";


const char *get_alu_inst_str(int i)
{
	switch(i)
	{
		ALU_CASE(ADD);
		ALU_CASE(SUB);
		ALU_CASE(AND);
		ALU_CASE(OR);

		ALU_CASE(XOR);
		ALU_CASE(NOT);
		ALU_CASE(SLL);
		ALU_CASE(SRL);
		ALU_CASE(SRA);

		ALU_CASE(DIV);
		ALU_CASE(DIVU);
		ALU_CASE(MUL);
		ALU_CASE(MULU);
		ALU_CASE(MULUS);
		ALU_CASE(MULHI);
		ALU_CASE(REM);
		ALU_CASE(CLE);
		ALU_CASE(CNE);
		ALU_CASE(CLT);
		ALU_CASE(CLTU);
		ALU_CASE(CEQ);
		DEFAULT;
	}
}

const char *get_jmp_inst_str(int i)
{
	switch(i)
	{
		JMP_CASE(JMP);
		JMP_CASE(BNE);
		JMP_CASE(BEQ);
		JMP_CASE(BLT);
		JMP_CASE(BLE);
		JMP_CASE(CALL);
		JMP_CASE(RET);
		DEFAULT;
	}
}

const char *get_mem_inst_str(int i)
{
	switch(i)
	{
		MEM_CASE(LDI);
		MEM_CASE(STI);
		MEM_CASE(PUSH);
		MEM_CASE(POP);
		MEM_CASE(SP);
		MEM_CASE(SFP);
		DEFAULT;
	}
}

const char *get_sys_inst_str(int i)
{
	switch(i)
	{
		DEFAULT;
	}
}




