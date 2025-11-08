#ifndef __MANAGER_HEADER__
#define __MANAGER_HEADER__

#include <stdbool.h>
#include "cli.h"

#define MAX_BREAKPOINTS 100

typedef struct file
{
	char *name;
	int id;
}file_t;



typedef struct globalstate
{
	int argc;
	char **argv;
	cli_context_t ctx;
	bool has_debuger;
	bool running;

	int runfor;
	int breakpoints[MAX_BREAKPOINTS];



}globalstate_t;





extern globalstate_t globalstate;

void step_handler(void);

void init(int argc, char **argv);

void update(void);

#endif


