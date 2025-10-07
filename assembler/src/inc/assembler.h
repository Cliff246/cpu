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

/*typedef enum asmval_type
{
	AV_T_UNKNOWN,
	AV_T_INTEGER,
	AV_T_FLOAT,
	AV_T_PTR,
}asmval_type_t;

typedef struct asmval_type_content
{

}asmval_type_content_t;

typedef enum asmval_sign
{
	AV_S_UNKNOWN,
	AV_S_SIGNED,
	AV_S_UNSIGNED,
}asmval_sign_t;

typedef struct asmval_sign_content
{

}asmval_sign_content_t;

typedef enum asmval_descriptor
{
	AV_D_UNKNOWN,
	AV_D_ARRAY,
	AV_D_STRING,
	AV_D_FUNC_PTR,
	AV_D_REFERENCE,
}asmval_descriptor_t;

typedef struct asmval_descriptor_content
{

}asmval_descriptor_content_t;

typedef enum asmval_size
{
	AV_8BIT,
	AV_16BIT,
	AV_32BIT,
	AV_64BIT,
}asmval_size_t;

typedef enum asmval_size_content
{

}asmval_size_content_t;

typedef enum asmval_modifier
{
	AV_M_UNKNOWN,
	AV_M_EMPTY,
	AV_M_CONST,
	AV_M_REALLOC,
	AV_M_VIOLATILE,
	AV_M_MUTABLE,
	AV_M_EXTERN,
}asmval_modifier_t;

typedef struct asmval_modifier_content
{
	asmval_modifier_t modifier;
	union
	{
		struct
		{

		}m_empty;
		struct
		{

		}m_constant;
		struct
		{

		}m_realloc;
		struct
		{
			uint64_t
		}m_volatile;
		struct
		{
			uint64_t segment;
		}m_mutable;
		struct
		{
			char *id;
		}m_extern;
	};
}asmval_modifier_content_t;

#define MAX_MODIFIERS 3

typedef struct asmval_intrinsic
{
	asmval_modifier_t mod;
	asmval_size_t size;
	asmval_type_t type;
	asmval_descriptor_t descptior;
	struct asmval_intrinsic *depth;

}asmval_intrinsic_t;
*/
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
