
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "flags.h"
#include "assembler.h"
#include "decoder.h"
#include "commons.h"
#include "error.h"
#include "lexer.h"
#define ARGUMENTS_COUNT 255

#include "fileio.h"






int main(int argc, char *argv[])
{
	char *content = NULL;
	int length = file_load(argv[1], &content);
	lexer_ctx_t *ctx = create_token_stream(content, 0);
	printf("step\n");

	for(int i = 0; i < ctx->count; ++i)
	{
		print_token(&ctx->toks[i]);
	}

	return 0;
	setup_errors();
	errno = 0;
	if(argc == 1)
	{
		printf("no argument's provided\n");
		return 1;
	}

	if(argc == 2)
	{
		char *content = NULL;
	   	int length = file_load(argv[1], &content);
		if(errno == EACCES && !content)
		{
			printf("bad file\n");
			return 1;
		}
		else
		{
			//printf("assemble\n");
			assemble(content, length, "bin");
			//test(content, length);
		}
		return 0;
	}


	int arguments[ARGUMENTS_COUNT];


	for(int i = 0; i < argc && i < ARGUMENTS_COUNT; ++i)
	{

	}

}
