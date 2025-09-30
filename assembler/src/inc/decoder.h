#ifndef __DECODER__
#define __DECODER__

#include <stdint.h>
#include <stdbool.h>



typedef enum error_code_inst
{
	valid,
	not_valid,	
	path_err,
	subpath_err,
	ref_err,
}errinst_t;

typedef struct instruction
{
	uint32_t path;
	uint32_t subpath;
	uint32_t rd;
	uint32_t rs1;
	uint32_t rs2;
	uint32_t aux;
	uint32_t immflag;
	
	uint64_t line;

	int64_t imm;
	char *linestr;
	char *immref;
	errinst_t err;
}inst_t;


void invalid_inst(char **splits, int length, inst_t *inst);
void inst_no_imm(char **splits, int length, inst_t *inst);
void inst_imm(char **splits, int length, inst_t *inst);





int get_path(char *keyword);
int get_subpath(int path,char *keyword);

int get_alu_subpath(char *keyword);
int get_mem_subpath(char *keyword);
int get_jmp_subpath(char *keyword);
int get_sys_subpath(char *keyword);



int get_register(char *keyword);


inst_t create_instruction(char *line, int linen);
void free_inst(inst_t *inst);
void print_inst(inst_t *inst);


inst_t decode_inst(int32_t code);
uint32_t encode_inst(inst_t *inst);
uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf);


#endif
