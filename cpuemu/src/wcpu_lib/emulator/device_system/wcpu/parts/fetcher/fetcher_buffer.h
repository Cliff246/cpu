#ifndef __WCPU_PART_FETCHER_BUFFER_HEADER__
#define __WCPU_PART_FETCHER_BUFFER_HEADER__

#include <stdint.h>
#include <stdbool.h>


#define FETCHER_BUFFER_CONTENT_SIZE 32


//quick fetcher ram for reordering instructions
typedef struct wcpu_fetcher_buffer
{
	uint64_t address;
	int64_t values[FETCHER_BUFFER_CONTENT_SIZE];
	int filled;
	bool used;
}fetcher_buffer_t;




#endif