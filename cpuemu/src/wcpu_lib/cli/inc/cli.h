#ifndef __CPU_CLI__
#define __CPU_CLI__

#include <stdlib.h>
#include "token.h"
#include "cmdsys.h"



typedef struct cli_context
{
	
	cmdline_t *lines;
	size_t count;
}cli_context_t;


cli_context_t *create_cli_context();

void pull_line(cli_context_t *ctx);


#endif
