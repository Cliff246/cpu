#ifndef __WCPU_PART_SIGNAL_AGGREGATOR_COMMAND_HEADER__
#define __WCPU_PART_SIGNAL_AGGREGATOR_COMMAND_HEADER__

#include "wcpu_part_signal_ptr.h"

#include <stdint.h>
#include <stdbool.h>

#include "aggregator.h"



struct wcpu_part_signal_AGGREGATOR_COMMAND
{
	aggregator_command_type_t type;
	//set the edge
	uint64_t edge;
	//the split location
	uint64_t split;
	//orgin of
	int orgin;
	//halt all
	bool halt;

};



#endif