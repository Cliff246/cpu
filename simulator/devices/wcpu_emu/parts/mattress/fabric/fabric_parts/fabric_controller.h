#ifndef __WCPU_FABRIC_PART_CONTROLLER_HEADER__
#define __WCPU_FABRIC_PART_CONTROLLER_HEADER__

#define FABRIC_CONTROL_COLUMNS 8
#define FABRIC_CONTROL_ROWS 8

#include "wcpu_isa.h"

#include <stdbool.h>

#define FABRIC_LOCAL_BUFFER_SIZE 16





typedef struct fabric_dependency_chain
{
	int rows[FABRIC_CONTROL_ROWS];
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



typedef struct wcpu_fabric_controller
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

}fabric_controller_t;


fabric_controller_t *create_fabric_control(void);




#endif