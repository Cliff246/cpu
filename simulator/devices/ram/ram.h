#ifndef __RAM_HEADER__
#define __RAM_HEADER__


#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

typedef struct WS_DEV_ram
{

	int64_t length;
	int64_t *content;

	bool has_msg;

	//make this a boolean array index'd by a ram flag enum
	bool print_content_flag;

	uint64_t local_address_start;
	uint64_t local_address_size;
	bool changed;

}DEV_ram_t;

#endif
