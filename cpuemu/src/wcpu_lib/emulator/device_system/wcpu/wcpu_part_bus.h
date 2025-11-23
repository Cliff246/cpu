#ifndef __WCPU_PART_BUS_HEADER__
#define __WCPU_PART_BUS_HEADER__

#include <stdlib.h>
#include <stdbool.h>
//X(SRC_PART, DEST_PART, NAME)

#define WCPU_PART_SIGNAL_LIST(X) 		\
	X(INVALID, INVALID, NONE)			\
	X(FETCHER, AGGREGATOR, PUSH)		\


#define WCPU_PART_SIGNAL_NAME(SRC, DST, TYPE) __WCPU_PART_SIGNAL_##SRC ## _TO ## _##DST ##_##TYPE
#define WCPU_PART_SIGNAL_ENUM(SRC, DST, TYPE) WCPU_PART_SIGNAL_NAME(SRC, DST, TYPE),



typedef enum wcpu_part_signal_type
{
	WCPU_PART_SIGNAL_LIST(WCPU_PART_SIGNAL_ENUM)
}part_signal_type_t;

typedef struct wcpu_part_signal
{
	part_signal_type_t signal_type;
	int src_id, dst_id;
	void *signal;

}part_signal_t;

#define CHANNEL_MAX_SIGNALS 50

typedef struct wcpu_part_channel
{
	part_signal_t *signals[CHANNEL_MAX_SIGNALS];
	int size;
	int index;
}part_channel_t;





typedef struct wcpu_part_bus
{

	part_channel_t import;
	part_channel_t export;

	bool dealt;


}part_bus_t;


bool push_signal_onto_channel(part_channel_t *channel, part_signal_t *signal);
bool pop_signal_off_channel(part_channel_t *channel, part_signal_t **signal);




#endif