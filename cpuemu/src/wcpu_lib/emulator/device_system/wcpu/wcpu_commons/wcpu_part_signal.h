#ifndef __WCPU_PART_SIGNAL_HEADER__
#define __WCPU_PART_SIGNAL_HEADER__

#include "wcpu_part_signal_ptr.h"
#include <stdbool.h>


typedef struct wcpu_part_signal
{
	part_signal_type_t signal_type;
	int src_id, dst_id;
	part_signal_content_ptr_t ptr;
	int ref_count;
	bool success;
	bool backlog;

}part_signal_t;
extern char *part_signal_strs[];

void part_signal_consume(part_signal_t **ptr);
void part_signal_release(part_signal_t **ptr);

part_signal_t *part_signal_create(part_signal_type_t type, int src_id, int dst_id, part_signal_content_ptr_t ptr);

void part_signal_print(part_signal_t *ptr);

#endif