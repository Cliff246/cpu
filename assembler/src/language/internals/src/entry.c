#include "entry.h"
#include "commons.h"
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
	if(entry->entry.mop.type == MOP_TYPE_DEFINE_CONFIG)
	{

	}
	else
	{
		entry->bytes = entry->entry.mop.holder.data.words_len * 8;

	}

}

void entry_macro(entry_t *entry)
{
	//TODO

}

entry_t *create_entry(parse_node_t *node)
{
	entry_t *entry = CALLOC(1, entry_t);

	entry->node = node;
	//printf("lexme %s\n", node->tok->lexeme);
	//fill out types
	if(node->kind == NODE_INSTR)
	{
		entry_instruction(entry);


	}
	else if(node->kind == NODE_METAOP)
	{
		entry_mop(entry);


	}
	else if(node->kind == NODE_MACRO)
	{

	}
	else
	{
		printf("entry is invalid\n");
		exit(1);
	}



	return entry;
}