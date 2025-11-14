

#include "arguments.h"
#include "commons.h"
#include "fileio.h"
#include <stdbool.h>
#include <string.h>


char *default_output = "a.bin";


void add_input_file(asmtarget_t *target, char *argv)
{

	if(!validate_path(argv))
	{
		printf("must be a valid path %s\n", argv);
		exit(1);
	}

	if(target->inputs == NULL)
	{
		target->inputs = CALLOC(1, char **);
		target->inputs[target->inputs_count++] = argv;
		return;
	}
	else
	{
		int index = target->inputs_count;
		target->inputs = REALLOC(target->inputs, target->inputs_count ++, char **);
		target->inputs[index] = argv;
	}
}


void set_output_file(asmtarget_t *target, char *output)
{
	if(target->output_file != NULL)
	{
		printf("can only have one output file: %s\n", output);
		exit(EXIT_FAILURE);
	}
	target->output_file = output;
}

void parse_assembler_args(asmtarget_t *target)
{
	char **argv = target->args.argv;
	int argc = target->args.argc;
	for(int i = 1; i < argc; ++i)
	{
		if(!strcmp(argv[i], "-o"))
		{
			if(i + 1 < argc)
			{
				set_output_file(target, argv[++i]);
			}
			else
			{
				printf("must set output file\n");
				exit(EXIT_FAILURE);
			}
		}
		else if(!strcmp(argv[i], "--debug"))
		{
			target->debug_enabled = true;
		}

		else
		{
			//shit ass set up
			add_input_file(target, argv[i]);
		}
	}
}

asmtarget_t target = {0};


void generate_target(int argc, char *argv[])
{

	target.args.argc = argc;
	target.args.argv = argv;

	parse_assembler_args(&target);


	if(target.output_file == NULL)
	{
		target.output_file = default_output;
	}

	if(target.inputs_count == 0)
	{
		printf("must have input sources\n");
		exit(EXIT_FAILURE);
	}


}