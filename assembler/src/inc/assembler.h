#ifndef __ASSEMBLER__
#define __ASSEMBLER__

#include <stdlib.h>
#include <stdint.h>

typedef struct 
{
	uint32_t *instructions;
	uint64_t *immedates;
	
	size_t immedates_len;
	size_t instructions_len;

	size_t immedates_alloc, instructions_alloc;	
}context_t;

typedef struct 
{
	char *str;
	size_t line;
	uint64_t address;
}reference_t;


void test(char *content, size_t length);
void assemble(char *content, size_t length);
#endif
