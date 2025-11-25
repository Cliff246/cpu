#include "wcpu_part_signal.h"
#include <stdlib.h>
#include <assert.h>

part_signal_t *part_signal_consume(part_signal_t *ptr)
{
	assert(ptr != NULL && "ptr cannot be null");


}

void part_signal_release(part_signal_t *ptr)
{
	assert(ptr != NULL && "ptr cannot be null");
	if(ptr->ref_count <= 0)
		free(ptr);
}
