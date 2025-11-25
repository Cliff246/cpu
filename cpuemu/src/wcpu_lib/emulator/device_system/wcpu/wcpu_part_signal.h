#ifndef __WCPU_PART_SIGNAL_HEADER__
#define __WCPU_PART_SIGNAL_HEADER__

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
	int ref_count;
}part_signal_t;

#endif