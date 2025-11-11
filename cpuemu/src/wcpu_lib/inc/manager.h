#ifndef __MANAGER_HEADER__
#define __MANAGER_HEADER__

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "cli.h"

#include <stdbool.h>

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


void fill_binary(uint64_t *bin, size_t length);
size_t file_len(FILE *fp);
void load_file(const char *file_name);

extern globalstate_t globalstate;

void step_handler(void);

void init(int argc, char **argv);

void update(void);

#endif


