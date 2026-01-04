#ifndef __OBJECT_TAG_HEADER__
#define __OBJECT_TAG_HEADER__

#include "commons.h"

#define OBJ_TAG_STRING_SIZE 20



#define OBJ_TAG_LIST(X) \
	X(UNKNOWN, char)	\
	X(BOOL, bool)		\
	X(UINT, uint64_t)	\
	X(INT, int64_t)		\
	X(STRING, char *)	\


#define OBJ_TAG_KEY_SIZE 25

#define OBJ_TAG_ENUM_NAME(X) OBJ_TAG_TYPE_ ## X
#define OBJ_TAG_ENUM(X, Y) OBJ_TAG_ENUM_NAME(X),

#define OBJ_TAG_COUNT (0 OBJ_TAG_LIST(COUNTER2D))

typedef enum WS_OBJ_tag_type
{
	OBJ_TAG_LIST(OBJ_TAG_ENUM)

}OBJ_tag_type_t;

#define OBJ_TAG_UNION(X, Y) Y X;


typedef union WS_OBJ_tag_union
{
	OBJ_TAG_LIST(OBJ_TAG_UNION)
}OBJ_tag_union_t;



typedef struct WS_OBJ_tag
{
	char key[OBJ_TAG_KEY_SIZE];
	OBJ_tag_type_t type;
	OBJ_tag_union_t value;
}OBJ_tag_t;

typedef struct WS_OBJ_tag_local_handle
{

}OBJ_tag_handle_t;

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