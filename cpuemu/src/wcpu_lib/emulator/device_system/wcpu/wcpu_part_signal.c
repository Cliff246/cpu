#include "wcpu_part_signal.h"
#include <stdlib.h>
#include <assert.h>

part_signal_t *part_signal_consume(part_signal_t *ptr)
{
	assert(ptr != NULL && "ptr cannot be null");

	ptr->ref_count++;
	return ptr;
}

void part_signal_release(part_signal_t *ptr)
{
	assert(ptr != NULL && "ptr cannot be null");
	if(ptr->ref_count <= 0)
	{
		//TODO this will fuck me in the future
		free(ptr->ptr.raw);
		free(ptr);
	}
}


part_signal_t *part_signal_create(part_signal_type_t type, int src_id, int dst_id, part_signal_content_ptr_t ptr)
{
	part_signal_t *signal = calloc(1, sizeof(part_signal_t));
	assert(signal != NULL && "signal malloc must succeed");


	signal->ref_count = 1;
	signal->ptr = ptr;
	signal->src_id = src_id;
	signal->dst_id = dst_id;
	signal->signal_type = type;
	return signal;
}