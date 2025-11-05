#include "entry.h"
#include "commons.h"
#include "fileio.h"
#include <stdio.h>

void entry_instruction(entry_t *entry)
{
	inst_t inst = create_instruction(entry->node);
	entry->entry.inst = inst;
	entry->type = ENTRY_INST;
	entry->bytes = 4;

	//print_inst(&entry->entry.inst);

}

void entry_mop(entry_t *entry)
{
	mop_t mop = create_mop(entry->node);
	entry->entry.mop = mop;
	entry->type = ENTRY_MOP;
	entry->bytes = entry->entry.mop.data.words_len * 8;



}

void entry_macro(entry_t *entry)
{
	//TODO

}

entry_t *create_entry(parse_node_t *node)
{
	entry_t *entry = CALLOC(1, entry_t);

	entry->node = node;
	//printf("lexme %s %s %d\n", node->tok->lexeme, get_filename_from_id(node->tok->locale.file), node->tok->locale.row);
	//fill out types
	if(node->kind == NODE_INSTR)
	{
		entry_instruction(entry);


	}
	else if(node->kind == NODE_METAOP)
	{
		entry_mop(entry);


	}
	else
	{
		printf("entry is invalid\n");
		exit(1);
	}



	return entry;
}


uint64_t get_line(entry_t *entry)
{
	return entry->node->tok->locale.row;
}


uint64_t get_file(entry_t *entry)
{
	return entry->node->tok->locale.file;
}




void print_entry(entry_t *entry)
{
	switch(entry->type)
	{
		case ENTRY_INST:
			print_inst(&entry->entry.inst);
			break;
		case ENTRY_MOP:
			print_mop(&entry->entry.mop);
			break;
		default:
			LOG("unknown entry\n", 0);
			break;

	}
}