
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
#define ARGUMENTS_COUNT 255
#include "inter.h"
#include "fileio.h"

#include "arguments.h"
#include "linker.h"
#include "emiter.h"



int main(int argc, char *argv[])
{

	setup_errors();
	errno = 0;

	generate_target(argc, argv);


	for(int filldesc = 0; filldesc < target.inputs_count; ++filldesc)
	{
		allocate_fdesc(target.inputs[filldesc]);
	}


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
			contexts[i] = load_context(get_fdesc(i));
			context_resolve(contexts[i]);
			add_context_to_linker(lk, contexts[i]);
		}
		//printf("validity %d\n", check_global_validity(lk));
		//print_globals(lk);
		//printf("%s\n", get_filename_from_context(contexts[0]));
		build_module_stack(lk);
		outorder_t *oo = create_outorder(lk);

		int buffer[100] = {0};
		create_link_order(lk, buffer, 100);
		//printf("\n\n");
		//for(int bufi = 0; bufi < 10; bufi++)
		//{
			//printf("%d \n", buffer[bufi]);
		//}


		set_outorder_positions(oo, buffer, oo->count);
		resolve_positions(oo);
		fix_addresses(lk, oo);
		//print_over_outorder(lk, oo);

		segout_t outs[oo->count];
		for(int ri = 0; ri < oo->count; ++ri)
		{
			outs[ri] = create_segout(lk, oo->regions[ri]);
		}
		output_t *out =  combine_segouts(outs, oo->count);
		write_out(out, target.output_file);
		//output_t *output = emit(contexts[0]);
		//write_out(output, (char *)target.output_file);
		//printf("assemble\n");
		//assemble(content, length, "bin");
		//test(content, length);
	}
	return 0;





}
