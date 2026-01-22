#ifndef __WCC_TOKEN_HEADER__
#define __WCC_TOKEN_HEADER__

#include "WCC_macros.h"
#include "WCC_string.h"

#include <stdint.h>


typedef enum WCC_token_type
{
	WCC_TOKEN_TYPE_ENUM(WCC_TOKEN_TYPE_LIST)
}WCC_toktype_t;

//designed to allow for consant values to point to the same data
typedef struct WCC_token_constant
{
	char const *ptr;
	uint32_t size;
	uint32_t id;
}WCC_tokconst_t;

//designed to allow for strings to 
typedef struct WCC_token_dynamic
{
	WCC_str_t str;
}WCC_tokdynm_t;



typedef struct WCC_token
{
	WCC_toktype_t type;
	bool constant;
	union
	{
		WCC_tokdynm_t dynamic;
		WCC_tokconst_t constant;
	}token;
}WCC_tok_t;

typedef struct WCC_token_constant_pool
{
	size_t size;
	WCC_tokconst_t *constants;

}WCC_tokconst_pool_t;

extern WCC_tokconst_pool_t pool;


#endif