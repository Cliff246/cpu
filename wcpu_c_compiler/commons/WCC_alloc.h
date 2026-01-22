#ifndef __WCC_ALLOC_HEADER__
#define __WCC_ALLOC_HEADER__

#include "WCC_constants.h"

#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>



 typedef struct WCC_memory_block
{
	struct WCC_memory_block *next;
	uint64_t size;
	int32_t free;
	char magic[4];

	_Alignas(64) uint8_t bytes[];
}WCC_mblock_t;


typedef struct WCC_memory_info
{
	pthread_mutex_t lock;
	uint64_t next_index;
	uint64_t total_size;
	uint64_t block_count;
	WCC_mblock_t *blocks;
}WCC_minfo_t;

extern WCC_minfo_t info;

typedef struct WCC_memory_ptr
{
	uint64_t size;
	WCC_mblock_t *block;
}*WCC_malias_t;


static int64_t _WCC_hash_to_elem(void *ptr);


static WCC_mblock_t *_WCC_find_free_block(WCC_mblock_t **last, uint64_t size);

static WCC_mblock_t *_WCC_request_space(WCC_mblock_t *last, uint64_t size);

void *WCC_malloc(uint64_t size);
void *WCC_free(void *mem);
void *WCC_calloc(uint64_t count, uint64_t size);
void *WCC_realloc(void *ptr, uint64_t size);


//get the contents of a WCC_malias_t
void *WCC_malias_ptr(WCC_malias_t alias);
//get
//
uint64_t WCC_malias_get(WCC_malias_t alias, uint64_t index, uint8_t stride);
//set
//attempts to set the value at the ptr[index * stride] = value[stride size]
//stride is a scaled number describing 
void WCC_malias_set(WCC_malias_t alias, uint64_t index, uint8_t stride, uint64_t value);


WCC_malias_t WCC_malias_alloc(uint64_t size);




#endif