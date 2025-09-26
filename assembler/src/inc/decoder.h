#ifndef __DECODER__
#define __DECODER__

#include <stdint.h>


typedef enum error_code_inst
{
	valid,
	not_valid,	
}errinst_t;

typedef struct instruction
{
	uint32_t path : 4;
	uint32_t subpath : 8;
	uint32_t rd : 5;
	uint32_t rs1 : 5;
	uint32_t rs2 : 5;
	uint32_t aux : 3;
	uint32_t immflag;
	
	uint64_t line;
	uint64_t imm;

	errinst_t err;
}inst_t;


int get_path(char *keyword);


int get_subpath(int path,char *keyword);

int get_alu_subpath(char *keyword);
int get_mem_subpath(char *keyword);
int get_jmp_subpath(char *keyword);
int get_sys_subpath(char *keyword);



int get_register(char *keyword);


inst_t create_instruction(char *line, int linen);



inst_t decode_inst(int32_t code);
int32_t encode_inst(inst_t *inst);
uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf);


#endif
