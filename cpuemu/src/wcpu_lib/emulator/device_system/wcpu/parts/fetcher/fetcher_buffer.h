#ifndef __WCPU_PART_FETCHER_BUFFER_HEADER__
#define __WCPU_PART_FETCHER_BUFFER_HEADER__

#include <stdint.h>
#include <stdbool.h>


#define FETCHER_BUFFER_CONTENT_SIZE 32

typedef enum wcpu_fetcher_buffer_type
{
	FETCHER_BUFFER_TYPE_INVAL,
	FETCHER_BUFFER_TYPE_INSTRUCTION,
	FETCHER_BUFFER_TYPE_IMMEDIATE,
	FETCHER_BUFFER_TYPE_CODE_DESCRIPTION,
}fetcher_buffer_type_t;


//quick fetcher ram for reordering instructions
typedef struct wcpu_fetcher_buffer
{
	uint64_t address;
	int64_t values[FETCHER_BUFFER_CONTENT_SIZE];
	int filled;
	fetcher_buffer_type_t type;
	bool used;
}fetcher_buffer_t;




#endif