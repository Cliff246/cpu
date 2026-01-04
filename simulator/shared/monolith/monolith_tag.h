#ifndef __MONOLITH_TAG_HEADER__
#define __MONOLITH_TAG_HEADER__

#include "commons.h"

#define MLTH_TAG_STRING_SIZE 20



#define MLTH_TAG_LIST(X) \
	X(UNKNOWN, char)	\
	X(BOOL, bool)		\
	X(UINT, uint64_t)	\
	X(INT, int64_t)		\
	X(STRING, char *)	\


#define MLTH_TAG_KEY_SIZE 25

#define MLTH_TAG_ENUM_NAME(X) MLTH_TAG_TYPE_ ## X
#define MLTH_TAG_ENUM(X, Y) MLTH_TAG_ENUM_NAME(X),

#define MLTH_TAG_COUNT (0 MLTH_TAG_LIST(COUNTER2D))

typedef enum WS_MLTH_tag_type
{
	MLTH_TAG_LIST(MLTH_TAG_ENUM)

}MLTH_tag_type_t;

#define MLTH_TAG_UNION(X, Y) Y X;


typedef union WS_MLTH_tag_union
{
	MLTH_TAG_LIST(MLTH_TAG_UNION)
}MLTH_tag_union_t;



typedef struct WS_MLTH_tag
{
	char key[MLTH_TAG_KEY_SIZE];
	MLTH_tag_type_t type;
	MLTH_tag_union_t value;
}MLTH_tag_t;

typedef struct WS_MLTH_tag_local_handle
{

}MLTH_tag_handle_t;

#endif
/*

#include "device_ptr.h"
#include "device_list.h"
#include "device_description.h"
#include "commons.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct device_description WS_dev_desc_t;










//goes into a hashmap is index and then executed via the function in it

#define WS_DEV_TAG_DEFAULT_LIST(X)\
	X(NAME, STRING)						   \
	X(ENABLED, BOOL)					   \

#define WS_DEV_TAG_DEFAULT_COUNT (0 WS_DEV_TAG_DEFAULT_LIST(COUNTER2D))





typedef bool (*WS_dev_tag_apply_fn)(WS_dev_t *device, WS_dev_tag_t *tag);


//should be the elements of a device
typedef struct
{
	char *id;
	WS_dev_tag_type_t expect;
	//applies tag to function
	WS_dev_tag_apply_fn fn;
} WS_dev_tag_producer_t;


//collections of tags used to send to device
typedef struct
{
	WS_dev_tag_t **tags;
	size_t tags_count;
	bool taken;
}WS_dev_cmd_collection_t;



typedef struct wcpu_simulator_device_command
{
	//used to expire this device ptr
	bool used;
	WS_dev_desc_t *type;

	//just one collection
	WS_dev_cmd_collection_t *collection;
}device_command_t, WS_dev_cmd_t;









#endif

*/