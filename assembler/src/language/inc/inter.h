


#ifndef __INTER__
#define __INTER__

#include "hashmap.h"
#include "lexer.h"
#include "parser.h"
#include <stdbool.h>
#include "strtools.h"
#include "decoder.h"
#include "fileio.h"

typedef enum isegment_type
{
	ISEG_NONE,
	ISEG_REALLOC,
	ISEG_TEXT,
	ISEG_DATA,
	ISEG_BSS,
}iseg_type_t;

typedef struct isegment
{
	iseg_type_t segtype;
	uint64_t segment_id;
	parse_node_t *head;
}iseg_t;

iseg_t *create_segment(parse_node_t *head);

typedef struct ireference
{
	char *ref_string;
	uint64_t segment_id;
	bool resolved;
	uint64_t resolved_address;
}iref_t;

typedef enum ientry_type
{
	IE_UNKNOWN,
	IE_INST,
	IE_MOP,
}ientry_type_t;

typedef struct
{

	parse_node_t *node;

	size_t offset;
	ientry_type_t type;
	union
	{
		inst_t inst;
		mop_t mop;
	}entry;


}ientry_t;


typedef struct iscope
{
	int segment_offset;
	ientry_t **entries;

	size_t entries_count;
	size_t entries_alloc;

	iref_t **refs;
	size_t refs_count;
	size_t refs_alloc;

	iseg_t *segment;

}iscope_t;


void fill_scope(iscope_t *scope, iseg_t *segment);
iref_t *create_reference(parse_node_t *head, iscope_t *scope, char *name);



ientry_t *create_entry(parse_node_t *node, iscope_t *scope);


uint64_t get_line(ientry_t *entry);
uint64_t get_file(ientry_t *entry);


typedef struct
{
	lexer_ctx_t *l_ctx;
	parser_ctx_t *p_ctx;
	parse_node_t *head;


	//holds the results of the ref_table ptr

	iscope_t *scopes;
	size_t scopes_count;

	p_hashtable_t ref_table;
	p_hashtable_t seg_table;

	file_desc_t *desc;

}icontext_t;


icontext_t *load_context(const char *path);
void context_resolve(icontext_t *ctx);




#endif
