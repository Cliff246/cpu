#ifndef __WCPU_PART_SIGNAL_HEADER__
#define __WCPU_PART_SIGNAL_HEADER__

#include "wcpu_part_signal_ptr.h"

typedef struct wcpu_part_signal
{
	part_signal_type_t signal_type;
	int src_id, dst_id;
	part_signal_content_ptr_t ptr;
	int ref_count;
}part_signal_t;

part_signal_t *part_signal_consume(part_signal_t *ptr);
void part_signal_release(part_signal_t *ptr);


#endif