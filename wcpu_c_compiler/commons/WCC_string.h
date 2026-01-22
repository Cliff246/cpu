#ifndef __WCC_STRING_HEADER__
#define __WCC_STRING_HEADER__

#include "WCC_alloc.h"
#include "WCC_memory.h"

#include <stdint.h>




typedef struct WCC_string
{
	uint32_t size;
	int32_t tag;
	WCC_sptr_t str;
}WCC_str_t;

typedef struct WCC_string_heap
{
	uint64_t count;
	WCC_str_t *strings;
}WCC_strheap_t;


extern WCC_strheap_t strheap;


#endif