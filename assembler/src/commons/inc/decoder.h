#ifndef __DECODER__
#define __DECODER__

#include <stdint.h>
#include <stdbool.h>

#include "parser.h"

typedef enum error_code_inst
{
	valid,
	not_valid,
	path_err,
	subpath_err,
	ref_err,
}errinst_t;

typedef enum inst_ref_type
{
	INST_REF_NONE,
	INST_REF_LOCAL,
	INST_REF_GLOBAL
}inst_ref_type_t;

typedef struct instruction
{
	uint32_t path;
	uint32_t subpath;
	uint32_t rd;
	uint32_t rs1;
	uint32_t rs2;
	char realocflag;
	char selflag;
	char immflag;

	uint64_t line;

	int64_t imm;
	char *linestr;

	char *immref;
	inst_ref_type_t ref_type;
	errinst_t err;
}inst_t;


typedef enum metaop_id
{
	MOP_UNKNOWN,
	MOP_I64,
	MOP_I32,
	MOP_I16,
	MOP_I8,
	MOP_FLOAT,
	MOP_DOUBLE,
	MOP_ALIGN,
	MOP_STRING,
	MOP_MEM,
	MOP_PTR,

	MOP_PUB,
	MOP_DEFINE,
	MOP_INCLUDE
}mop_id_t;

typedef enum mop_type
{
	MOP_TYPE_UNKNOWN,
	MOP_TYPE_DEFINE_DATA,
	MOP_TYPE_DEFINE_CONFIG,
}mop_type_t;

typedef struct data_holder
{

	uint64_t *words;
	size_t words_len;
}data_holder_t;

typedef enum config_type
{
	MOP_CONFIG_TYPE_INCLUDE,
	MOP_CONFIG_TYPE_PUBLIC,
	MOP_CONFIG_TYPE_DEFINE,

}config_type_t;

typedef struct config
{
	config_type_t type;

	union
	{

	} config_type;
}config_t;

typedef struct config_holder
{
	config_t configuration;
}config_holder_t;


data_holder_t decode_string(parse_node_t *head);
data_holder_t decode_integer(parse_node_t *head);



data_holder_t create_data_holder(parse_node_t *node);




typedef struct metaop
{
	mop_id_t mop;
	mop_type_t type;


	char *mop_id;

	parse_node_t **expressions;
	size_t expressions_len;


	union
	{
		data_holder_t data;
		config_holder_t config;
	}
	holder;

}mop_t;



void invalid_inst(parse_node_t *node, inst_t *inst);


void inst_no_imm(parse_node_t *node, inst_t *inst);


void inst_imm(parse_node_t *node, inst_t *inst);

int get_path(char *keyword);
int get_subpath(int path,char *keyword);

int get_alu_subpath(char *keyword);
int get_mem_subpath(char *keyword);
int get_jmp_subpath(char *keyword);
int get_sys_subpath(char *keyword);





mop_id_t get_mop_code(char *keyword);


int get_register(char *keyword);

mop_t create_mop(parse_node_t *node);
inst_t create_instruction(parse_node_t *node);
void free_inst(inst_t *inst);
void print_inst(inst_t *inst);


inst_t decode_inst(uint32_t code);
uint32_t encode_inst(inst_t *inst);
uint64_t encode(uint64_t path, uint64_t subpath, uint64_t rd, uint64_t rs1, uint64_t rs2, uint64_t aux, uint64_t immf);



#endif
