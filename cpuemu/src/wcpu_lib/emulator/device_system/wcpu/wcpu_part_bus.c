#include "wcpu_part_bus.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


//
bool push_signal_onto_channel(part_channel_t *channel, part_signal_t *signal)
{
	if(channel->size >= CHANNEL_MAX_SIGNALS || channel->size < 0)
	{
		fprintf(stderr,"to many signals on channel %d ", channel->size);
		return false;
	}
	channel->signals[channel->size++] = signal;
	return true;
}


//
bool pop_signal_off_channel(part_channel_t *channel, part_signal_t **signal)
{
	if(channel->size <= 0 || channel->size >= CHANNEL_MAX_SIGNALS)
	{

		return false;
	}

	*signal = channel->signals[channel->size--];
	return true;
}