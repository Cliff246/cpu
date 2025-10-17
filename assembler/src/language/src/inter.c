#include "inter.h"
#include "commons.h"
#include <stdlib.h>
#include <string.h>



void entry_instruction(ientry_t *entry)
{
	inst_t inst = create_instruction(entry->node);
	entry->entry.inst = inst;
	entry->type = IE_INST;

	print_inst(&entry->entry.inst);

}

void entry_mop(ientry_t *entry)
{
	mop_t mop = create_mop(entry->node);
	entry->entry.mop = mop;
	entry->type = IE_MOP;
}

ientry_t *create_entry(parse_node_t *node, iscope_t *scope)
{
	ientry_t *entry = CALLOC(1, ientry_t);

	entry->node = node;
	entry->offset = scope->segment_offset;
	//TODO
	if(scope->entries_alloc == scope->entries_count)
	{
		scope->entries = REALLOC(scope->entries, scope->entries_alloc *= 2, ientry_t *);
	}
	//fill out types
	if(node->kind == NODE_INSTR)
	{
		entry_instruction(entry);

	}
	else if(node->kind == NODE_METAOP)
	{
		entry_mop(entry);

	}
	scope->entries[scope->entries_count++] = entry;
	scope->segment_offset ++;


	return entry;
}

iseg_type_t get_seg_type(char *node)
{

	const char *segment_types[] = {
		".realloc",
		".text",
		".data",
		".bss"
	};
	iseg_type_t type = 0;
	for(int i = 0; i < ARYSIZE(segment_types); ++i)
	{
		if(!strcmp(node, segment_types[i]))
		{
			type = i + 1;
		}
	}
	return type;
}

iseg_t *create_segment(parse_node_t *head)
{
	static int segid = 0;
	iseg_t *segment = CALLOC(1, iseg_t);

	segment->head = head;
	segment->segment_id = segid++;
	//find type
	//TODO
	//segment->segtype =
	//
	//
	//
	segment->segtype = get_seg_type(head->tok->lexeme);
	return segment;
}

iref_t *create_reference(parse_node_t *head, iscope_t *scope, char *name)
{
	iref_t *ptr_ref = CALLOC(1, iref_t);
	if(scope->refs_alloc == scope->refs_count)
	{
		scope->refs = REALLOC(scope->refs, scope->refs_alloc *= 2, iref_t *);
	}
	scope->refs[scope->refs_count++] = ptr_ref;
	ptr_ref->ref_string = head->tok->lexeme;
	ptr_ref->resolved = false;
	ptr_ref->resolved_address = false;
	ptr_ref->segment_id = scope->segment->segment_id;

	return ptr_ref;
}

void fill_scope(iscope_t *scope, iseg_t *segment)
{

	scope->entries_alloc = 1000;
	scope->entries_count = 0;
	scope->entries = CALLOC(scope->entries_alloc, ientry_t *);

	scope->refs_alloc = 100;
	scope->refs_count = 0;
	scope->refs = CALLOC(scope->refs_alloc, iref_t *);

	scope->segment = segment;
	scope->segment_offset = 0;
}


uint64_t get_line(ientry_t *entry)
{
	return entry->node->tok->locale.row;
}

uint64_t get_file(ientry_t *entry)
{
	return entry->node->tok->locale.file;

}

void free_iref(void *ptr)
{
	iref_t *ref = (iref_t *)ptr;
	free(ref->ref_string);
	free(ref);
}

void free_iseg(void *ptr)
{
	iseg_t *seg = (iseg_t *)ptr;
	free(seg);
}

//gives a temporary string for look up
char *segment_id_to_string(size_t segid)
{
	const int size = 33;
	static char number[size] = {0};
	memset(number, 0, size * sizeof(char));
 	sprintf(number, "%lu", segid);
	return number;
}



icontext_t *load_context(const char *path)
{
	icontext_t *ctx = CALLOC(1, icontext_t);
	static int fileid = 0;
	fileid++;
	file_desc_t *desc = get_fdesc(path);

	ctx->desc = desc;

	ctx->l_ctx = create_token_stream(desc->src, fileid);





	ctx->p_ctx = create_context(ctx->l_ctx);
	ctx->head = parse_program(ctx->p_ctx);


	ctx->scopes_count = ctx->head->child_count;
	ctx->scopes = CALLOC(ctx->scopes_count, iscope_t);



	const int max_size = 10000;
	ctx->ref_table = new_hash_table(max_size, free_iref);
	ctx->seg_table = new_hash_table(max_size, free_iseg);

	return ctx;
}




void context_resolve(icontext_t *ctx)
{
	print_depth(ctx->head, 0);
	//current semgents
	for(int s = 0; s < ctx->head->child_count; ++s)
	{
		//segments
		parse_node_t *cur = ctx->head->children[s];
		iscope_t *scope = &ctx->scopes[s];

		fill_scope(scope, create_segment(cur));

		addto_hash_table(ctx->seg_table, segment_id_to_string(scope->segment->segment_id), scope->segment);

		for(int r = 0; r < cur->child_count; ++r)
		{
			//references
			parse_node_t *subcur = cur->children[r];

			char *name = subcur->tok->lexeme;


			iref_t *check = getdata_from_hash_table(ctx->ref_table, name);
			if(check != NULL)
			{
				//duplicate's in the hash table

			}

			iref_t *ref = create_reference(subcur,scope, name);



			addto_hash_table(ctx->ref_table, ref->ref_string, (void *)ref);

			for(int i = 0; i < subcur->child_count; ++i)
			{
				create_entry(subcur->children[i], scope);
			}

		}

	}
}
