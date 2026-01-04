#ifndef __DYNDATA__
#define __DYNDATA__

#include "commons.h"
#include <stdint.h>
#include <stdio.h>

#define DYNDATA_TYPE_LIST(X)			\
	X(UINT8		, BASE)  	\
	X(UINT16	, BASE) 	\
	X(UINT32	, BASE) 	\
	X(UINT64	, BASE) 	\
	X(INT8		, BASE)    	\
	X(INT16		, BASE)  	\
	X(INT32		, BASE)  	\
	X(INT64		, BASE)  	\
	X(BOOL		, BASE)		\
	X(FLOAT		, BASE)		\
	X(DOUBLE	, BASE)		\
	X(LIST		, ARRAY)	\
	X(STRING	, ARRAY)	\
	X(STRUCT	, COMPOUND)	\

#define DYNDATA_TYPE_ENUM_NAME(X) DYNDATA_TYPE_ ## X
#define DYNDATA_TYPE_ENUM(X, Y) DYNDATA_TYPE_ENUM_NAME(X),

typedef enum
{
	DYNDATA_TYPE_LIST(DYNDATA_TYPE_ENUM)
}dyntype_t;

typedef struct
{

}dyndata_t;


#endif