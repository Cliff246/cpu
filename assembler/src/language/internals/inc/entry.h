#ifndef __ENTRY_HEADER__
#define __ENTRY_HEADER__

#include <stdint.h>
#include <stdbool.h>


#include "parser.h"
#include "decoder.h"
#include "instruction.h"
#include "metaop.h"
#include "psudeoop.h"


typedef enum entry_type
{
	ENTRY_UNKNOWN,
	ENTRY_INST,
	ENTRY_MOP,
	ENTRY_POP,
}entry_type_t;




typedef struct
{

	parse_node_t *node;

	size_t bytes;
	entry_type_t type;
	bool error;
	union
	{
		inst_t inst;
		mop_t mop;
		pop_t pop;

	} entry;


}entry_t;




entry_t *create_entry(parse_node_t *node);


uint64_t get_line(entry_t *entry);
uint64_t get_file(entry_t *entry);

void print_entry(entry_t *entry);

#endif

