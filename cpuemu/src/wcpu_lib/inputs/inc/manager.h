#ifndef __MANAGER_HEADER__
#define __MANAGER_HEADER__

#include <stdbool.h>
#include "cli.h"

typedef struct globalstate
{
	int argc;
	char **argv;
	cli_context_t ctx;
	bool has_debuger;



}globalstate_t;


extern globalstate_t globalstate;


void init(int argc, char **argv);

void update(void);

#endif


