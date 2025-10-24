


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
	ISEG_REALLOC,		//realloc segments
	ISEG_TEXT,			//code semgents that are seperate
	ISEG_DATA,			//data segments
	ISEG_BSS,			//bss
	ISEG_CODE,			//combined unified code blocks
	ISEG_CONFIG,
}iseg_type_t;


typedef struct isegment_code
{
	int id;
}iseg_code_t;


typedef struct isegment_config
{
	char **includes;
	char **public;
	char **defines;
	size_t includes_len, public_len, defines_len;
}iseg_config_t;


typedef struct isegment_text
{

}iseg_text_t;


typedef struct isegment_realloc
{

}iseg_realloc_t;



typedef struct isegment_data
{

}iseg_data_t;


typedef struct isegment
{
	iseg_type_t segtype;
	uint64_t segment_id;

	parse_node_t *head;

	union
	{
		iseg_code_t code;
		iseg_config_t config;
		iseg_text_t text;
		iseg_realloc_t realloc;
		iseg_data_t data;
	}content;

}iseg_t;

iseg_t *create_segment(parse_node_t *head);
iseg_code_t create_code_segment(iseg_t *seg);
iseg_config_t create_config_segment(iseg_t *seg);
iseg_text_t create_text_segment(iseg_t *seg);
iseg_data_t create_data_segment(iseg_t *seg);
iseg_realloc_t create_realloc_segment(iseg_t *seg);


typedef struct ireference
{
	char *ref_string;
	uint64_t offset;
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

	} entry;


}ientry_t;


typedef struct iscope
{
	int offset;
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

	file_desc_t *desc;

}icontext_t;


icontext_t *load_context(file_desc_t *desc);
void context_resolve(icontext_t *ctx);



#endif
