#ifndef __FABRIC_CONTROLLER_HEADER__
#define __FABRIC_CONTROLLER_HEADER__

#define FABRIC_CONTROL_COLUMNS 8
#define FABRIC_CONTROL_ROWS 8

#include "fabric_register.h"
#include "wcpu_isa.h"



#define FABRIC_LOCAL_BUFFER_SIZE 16

typedef struct fabric_instruction
{
	//oversized
	uint16_t opcode;
	//destination
	frreg_t dst;
	//register source1
	frreg_t rs1;
	//register source2
	frreg_t rs2;
	//register source3
	frreg_t rs3;
	//immediate index in side channed of the immediate
	int16_t immindex;
	int16_t pc;

	char uses_acc, edge;



}fabinst_t;


bool fabric_instruction_is_jump(fabinst_t *inst);
bool fabric_instruction_is_load(fabinst_t *inst);
bool fabric_instruction_is_store(fabinst_t *inst);
bool fabric_instruction_is_ready(fabinst_t *inst);

fabinst_t create_fabric_instruction(inst_t);



typedef struct fabric_dependency_chain
{
	fabinst_t rows[FABRIC_CONTROL_ROWS];
	//the lowest instruction in the column
	int lowest;
	//the dependency of the column
	int dep;
}fab_deps_chain_t;

typedef struct fabric_cache_entry
{
	int64_t imm;
	int16_t immindex;
}fabric_cache_entry_t;



typedef struct fabric_control
{
	fabric_cache_entry_t local_cache[FABRIC_LOCAL_BUFFER_SIZE];

	fab_deps_chain_t input_table[FABRIC_CONTROL_COLUMNS];

	//has load
	int has_load;
	//has store
	int has_store;

	//has jump instruction
	int has_jump;
	//armed jump
	int armed;

	//cycles since last op code
	int cycles_since_last;
	//error code
	char has_error;
	//is stalled
	int stall;
	//is ready
	int ready;

}fabric_control_t;


fabric_control_t *create_fabric_control(void);




#endif