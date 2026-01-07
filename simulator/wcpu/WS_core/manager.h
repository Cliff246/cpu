#ifndef __MANAGER_HEADER__
#define __MANAGER_HEADER__


#include "cli.h"
#include "SIM_simulator.h"
#include "dynamic_lib.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define MAX_BREAKPOINTS 100
#define MAX_EMUARGUMENTS 100
#define MAX_SIMULATORS 100


#define FLAG_TYPE_ENUM(X)	\
	X(EMPTY)				\
	X(HAS_SOURCE)			\
	X(RUNNING)				\
	X(HAS_DEBUGGER)			\
	X(EXPORT)				\
	X(IGNORE_BREAK)			\
	X(TESTING)				\
	X(ONLY_RUNFOR)			\


#define EX(val) FLAG_ ## val,

typedef enum globalstate_flag
{
	FLAG_TYPE_ENUM(EX)
}gsflag_t;



#define CX(val) + 1

#define GSFLAG_COUNT (0 FLAG_TYPE_ENUM(CX))

typedef struct arguments
{
	int argc;
	char **argv;



}global_arguments_t;

typedef struct breakpoint
{
	int index;
}brkpnt_t;


typedef struct loaded_modules
{
	WS_dynamic_lib_t **dynamic_libs;

	size_t count;
}loaded_modules_t;

static void load_module(const char *module);


typedef struct globalstate
{

	global_arguments_t args;


	int runfor;
	int breakpoints[MAX_BREAKPOINTS];

	bool flags[GSFLAG_COUNT];

	char *export_path;

	char *source_path;

	loaded_modules_t loaded;

}globalstate_t;


bool get_flag(gsflag_t flag);
void set_flag(gsflag_t flag);
void clr_flag(gsflag_t flag);

//void basic_export(void);

extern globalstate_t globalstate;

//void step_handler(void);
void parse_args(void);

void init(int argc, char **argv);

//void update(void);
//void testing(void);


#endif


