#include "wcpu_part_signal.h"
#include "wcpu_part.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


#define PART_SIGNAL_STRS(X) [PART_SIGNAL_ENUM_NAME(X)] = #X,

char *part_signal_strs[] =
{
	PART_SIGNAL_LIST(PART_SIGNAL_STRS)
};

void part_signal_consume(part_signal_t **ptr)
{
	assert(ptr != NULL && "ptr cannot be null");
	//printf("consume %d\n",(*ptr)->ref_count);
	//(*ptr)->ref_count++;
}

void part_signal_release(part_signal_t **ptr)
{
	assert(*ptr != NULL && "ptr cannot be null");
	//(*ptr)->ref_count--;
	if((*ptr)->ref_count <= 0)
	{
		//printf("free ptr\n");
		//TODO this will fuck me in the future
		free((*ptr)->ptr.raw);
		free(ptr);
		*ptr = NULL;
	}
	return;
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

void part_signal_print(part_signal_t *ptr)
{
	if(ptr == NULL)
		return;
	printf("signal: dst:%d src:%d signal_type:%s\n", ptr->dst_id, ptr->src_id, part_signal_strs[ptr->signal_type]);
}