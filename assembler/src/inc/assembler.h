#ifndef __ASSEMBLER__
#define __ASSEMBLER__

#include <stdlib.h>
#include <stdint.h>
#include <hashmap.h>
#include <decoder.h>

typedef enum segment_type
{
	SEG_INVAL,
	SEG_TEXT,
	SEG_REALLOC,
	SEG_DATA,
}segment_type_t;


typedef struct segment
{

	uint64_t range;
	uint64_t start;
	segment_type_t segtype;

}segment_t;

segment_type_t get_segment_type(char *str, size_t max);

typedef enum reference_type
{
	REF_INVAL,
	REF_TEXT,
	REF_REALLOC,
	REF_DATA,
	REF_FUNC
}reference_type_t;



typedef struct reference
{
	reference_type_t rtype;
	char *str;
	size_t line;
	uint64_t address;
	int segment_id;

}reference_t;


typedef enum
{
	//invalid type
	CL_INVALID,
	//reference
	CL_REFERENCE,
	//comment
	CL_COMMENT,
	//instruction
	CL_INSTRUCTION,
	//segment descriptor
	CL_SEGMENT,
	//meta op
	CL_MOP,
	//continued meta op
	CL_MOP_CONT,
}codeline_t;


typedef struct asmlines
{
	int ref_size;
	hashtable_t *ref_table;
	size_t lines_size;
	codeline_t *lines_ary;
}asmlines_t;

typedef struct asmdecode
{

	//instructions length allocated
	size_t instr_len;
	//instructions array;
	inst_t *instr_ary;

	//the immedates
	size_t imm_len;
	int64_t *imm_ary;

	size_t table_offsets_len;
	//the binary size of this
	uint64_t table_bin_len;
	uint64_t *table_offsets_ary;

	//
	int64_t instr_iter;
	int64_t imm_iter;
	uint64_t table_offsets_iter;
}asmdecode_t;


typedef struct asmbinary
{

	uint32_t *instructions;
	int64_t *immedates;

	//
	size_t immedates_len;
	size_t instructions_len;

	//dynamic array alloc
	size_t immedates_alloc, instructions_alloc;

	//code table len
	uint64_t table_len;
	uint64_t *table;
}asmbinary_t;



typedef struct asminter
{
	segment_t *seg_ary;
	reference_t *ref_ary;

	size_t seg_ary_size;
	size_t ref_ary_size;
}asminter_t;



typedef struct entry
{
	//type of entry
	int type;
	//content of the entry
	char *content;
	uint64_t content_len;
	uint64_t line_number;
	reference_t *ref;

}entry_t;



typedef struct asmval
{
	entry_t *internals;
	size_t lines;


}asmval_t;

typedef struct asmdata
{
	asmval_t *entries;
	size_t entries_len;
}asmdata_t;

typedef struct
{


	size_t segments_len;
	segment_t *segments;


	size_t refs_len;
	reference_t *refs;
	//split_lines
	char **splitlines;
	size_t splitlines_len;


	asmlines_t lines;
	asmdecode_t decode;
	asmbinary_t *binary;
}context_t;

entry_t *new_entry(context_t *ctx, reference_t *ref, uint64_t ln_start, uint64_t ln_end);
void free_entry(entry_t *ptr);


codeline_t enscribe_line(context_t *ctx, char *line, codeline_t last);

void map_ref(context_t *ctx, int address, int segment_index);
void map_seg(context_t *ctx, int segment_index);
void map_inst(context_t *ctx);
void map_mop(context_t *ctx, char *line, int segment_index);

void print_reference(reference_t *ref);



void first_stage(context_t *context, char *content, size_t length);
asmbinary_t *init_binary_buffer(size_t alloc_init);

asmlines_t determine_lines(context_t *ctx);
int get_following_indented(char **lines, int length, int index);

void init_asm_decode(context_t *ctx);

void test(char *content, size_t length);
void assemble(char *content, size_t length, char *file_name);
#endif
