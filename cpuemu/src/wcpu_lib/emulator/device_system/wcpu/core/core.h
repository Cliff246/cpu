#ifndef __CORE_HEADER__
#define __CORE_HEADER__

#include "common.h"

#include "wcpu_part.h"
#include "wcpu_part_signal_ptr.h"

#include "wcpu_part_signal_core_io.h"

#include <stdint.h>
#include <stdlib.h>




#define CORE_PART_ORDER(X, Y) [CORE_PART_NAME(X)] = Y,


#define CORE_STATE_LIST(X)			\
	X(INVAL)						\
	X(DEBUG)						\
	X(STOPPED)						\
	X(RUNNING)						\
	X(INIT)							\


#define CORE_STATE_NAME(X) CORE_STATE_ ## X

#define CORE_STATE_ENUM(X)	CORE_STATE_NAME(X),

typedef enum wcpu_core_state
{
	CORE_STATE_LIST(CORE_STATE_ENUM)
}core_state_t;


typedef enum wcpu_core_io_type
{
	CORE_IO_NONE,

	CORE_IO_READ,
	CORE_IO_WRITE,
}core_io_type_t;

typedef struct wcpu_core
{
	core_state_t state;
	part_t *parts[UNIQUE_PARTS];
	bool startup;

	//this is inefficent but oh well
	//stores all the





	//TODO determine if this is going to block writes with only one
	struct
	{
		uint64_t address;
		int64_t value;
		bool issued;
		core_io_type_t type;
		bool responded;
	}core_io;

}core_t;



void wcpu_core_clear_io(core_t *core);
static core_t *wcpu_create_core(void);

core_t *wcpu_core_generate(void);
void wcpu_core_update(core_t *core);

void wcpu_core_handle_messages(core_t *core);


#endif
