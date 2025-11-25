#ifndef __WCPU_PART_BUS_HEADER__
#define __WCPU_PART_BUS_HEADER__

#include <stdlib.h>
#include <stdbool.h>

#include "wcpu_part_signal.h"

#define CHANNEL_MAX_SIGNALS 50

typedef struct wcpu_part_channel
{
	part_signal_t *signals[CHANNEL_MAX_SIGNALS];
	int size;
	int index;
}part_channel_t;




//the bus of the
typedef struct wcpu_part_bus
{

	part_channel_t import;
	part_channel_t export;

	bool dealt;


}part_bus_t;


bool push_signal_onto_channel(part_channel_t *channel, part_signal_t *signal);
bool pop_signal_off_channel(part_channel_t *channel, part_signal_t **signal);




#endif