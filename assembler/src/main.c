
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
#include "linker.h"




int main(int argc, char *argv[])
{

	setup_errors();
	errno = 0;

	generate_target(argc, argv);





	if(errno == EACCES)
	{
		printf("bad file\n");
		return 1;
	}
	else
	{
		context_t **contexts = CALLOC(target.inputs_count, context_t*);

		linker_t *lk = create_linker();
		for(int i = 0; i < target.inputs_count; ++i)
		{
			printf("i:%d\n", i);
			char *src = target.inputs[i];
			contexts[i] = load_context(get_fdesc(src));
			context_resolve(contexts[i]);
			add_context_to_linker(lk, contexts[i]);
		}
		printf("validity %d\n", check_global_validity(lk));
		printf("%s\n", contexts[0]->desc->name);
		output_t *output = emit(contexts[0]);
		write_out(output, (char *)target.output_file);
		//printf("assemble\n");
		//assemble(content, length, "bin");
		//test(content, length);
	}
	return 0;





}
