#include "wcpu_part_bus.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


//
bool push_signal_onto_channel(part_channel_t *channel, part_signal_t *signal)
{
	//printf("channel index: %d\n", channel->index);
	if(channel->index >= CHANNEL_MAX_SIGNALS || channel->index < 0)
	{
		printf("to many signals on channel %d\n", channel->index);
		return false;
	}
	channel->signals[channel->index++] = signal;
	part_signal_consume(&signal);

	return true;
}


//
bool pop_signal_off_channel(part_channel_t *channel, part_signal_t **signal)
{

	if(channel->index <= 0 || channel->index >= CHANNEL_MAX_SIGNALS)
	{

		return false;
	}

	*signal = channel->signals[--channel->index];
	part_signal_release(signal);

	return true;
}

void print_part_channel(part_channel_t *channel)
{
	for(int i = 0; i < channel->index; ++i)
	{
		part_signal_print(channel->signals[i]);
	}
}