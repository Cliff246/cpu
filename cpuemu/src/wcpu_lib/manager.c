


#include "manager.h"
#include <stdbool.h>

globalstate_t globalstate =
{
	.argc = 0,
	.argv = 0,
	.ctx = {0},
	.has_debuger = false
};

void init(int argc, char **argv)
{

	globalstate.argc = argc;
	globalstate.argv = argv;
	create_cli_context(&globalstate.ctx);
}


void update(void)
{
	
}