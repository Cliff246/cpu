#ifndef __DEVICE_COMMAND_HEADER__
#define __DEVICE_COMMAND_HEADER__

#include "device.h"
#include "device_list.h"
#include "device_description.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct device_description WS_dev_desc_t;



#define WS_DEV_CMD_FLAG_LIST(X) \
	X(UNKNOWN, char)			   \
	X(BOOL, bool)				   \
	X(UINT, uint64_t)			   \
	X(INT, int64_t)				   \
	X(STRING, char *)			\


#define WS_DEV_CMD_FLAG_ENUM_NAME(X) WS_DEV_CMD_FLAG_TYPE_ ## X
#define WS_DEV_CMD_FLAG_ENUM(X, Y) WS_DEV_CMD_FLAG_ENUM_NAME(X),

typedef enum
{
	WS_DEV_CMD_FLAG_LIST(WS_DEV_CMD_FLAG_ENUM)

}WS_dev_cmd_flag_type_t;

#define WS_DEV_CMD_FLAG_UNION(X, Y) Y X;

typedef union
{
	WS_DEV_CMD_FLAG_LIST(WS_DEV_CMD_FLAG_UNION)
}WS_dev_cmd_flag_value_t;


//goes into a hashmap is index and then executed via the function in it




typedef struct
{
	char *flag;
	WS_dev_cmd_flag_type_t type;
	WS_dev_cmd_flag_value_t *value;
}WS_dev_cmd_flag_t;


typedef bool (*WS_dev_cmd_flag_apply_fn)(WS_dev_t *device, WS_dev_cmd_flag_t *flag);


//should be the elements of a device
typedef struct
{
	char *id;
	WS_dev_cmd_flag_type_t expect;
	//applies flag to function
	WS_dev_cmd_flag_apply_fn fn;
} WS_dev_cmd_flag_producer_t;


//collections of flags used to send to device
typedef struct
{
	WS_dev_cmd_flag_t *flags;
	size_t flags_count;
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