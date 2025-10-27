
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "flags.h"
#include "decoder.h"
#include "commons.h"
#include "eerror.h"
#include "lexer.h"
#include "parser.h"
#include "encoder.h"
#define ARGUMENTS_COUNT 255
#include "inter.h"
#include "fileio.h"

#include "arguments.h"





int main(int argc, char *argv[])
{

	setup_errors();
	errno = 0;

	generate_target(argc, argv);

	char *first_source = target.inputs[0];

	file_desc_t *desc = get_fdesc(first_source);
	if(errno == EACCES && !desc)
	{
		printf("bad file\n");
		return 1;
	}
	else
	{

		context_t *context = load_context(desc);
		context_resolve(context);

		output_t *output = emit(context);
		write_out(output, (char *)target.output_file);
		printf("assemble\n");
		//assemble(content, length, "bin");
		//test(content, length);
	}
	return 0;





}
