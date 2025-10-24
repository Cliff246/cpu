#ifndef __ASM_ARGUMENTS__
#define __ASM_ARGUMENTS__

#include <stdbool.h>

typedef struct c_args
{
	int argc;
	char **argv;
}c_args_t;


extern char *default_output;



typedef struct asmtarget
{
	const char *output_file;

	char **inputs;
	int inputs_count;


	bool debug_enabled;
	c_args_t args;
}asmtarget_t ;

extern asmtarget_t target;


void parse_assembler_args(asmtarget_t *target);

void generate_target(int argc, char *argv[]);



#endif