#ifndef __ASSEMBLER__
#define __ASSEMBLER__

#include <stdlib.h>
#include <stdint.h>

typedef struct 
{
	uint32_t *instructions;
	int64_t *immedates;
	
	size_t immedates_len;
	size_t instructions_len;

	size_t immedates_alloc, instructions_alloc;	


	uint64_t table_len;
	uint64_t *table;
}context_t;


typedef enum reference_type
{
	REF_INVAL,
	REF_
}reference_type_t;

typedef struct 
{
	char *str;
	size_t line;
	uint64_t address;
}reference_t;



void test(char *content, size_t length);
void assemble(char *content, size_t length, char *file_name);
#endif
