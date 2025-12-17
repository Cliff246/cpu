#ifndef __DEVICE_VTABLE_HEADER__
#define __DEVICE_VTABLE_HEADER__

#include "device.h"
#include "token.h"

typedef struct wcpu_simulator_device WS_dev_t;
typedef struct wcpu_simulator_device_command WS_dev_cmd_t;
typedef struct wcpu_simulator_device_message WS_dev_msg_t;

typedef void (*WS_device_init_fn)(WS_dev_t *dev, WS_dev_cmd_t *cmd);
//complete execution
typedef void (*WS_device_update_fn)(WS_dev_t *dev);
//read from
typedef bool (*WS_device_read_fn)(WS_dev_t *dev, WS_dev_msg_t *msg);
typedef WS_dev_msg_t *(*WS_device_send_fn)(WS_dev_t *dev);
typedef void (*WS_device_print_fn)(WS_dev_t *dev);
//should generate void *setting for config
typedef WS_dev_cmd_t *(*WS_device_cmd_stringfy_fn)(WS_dev_t *dev, char *string);

typedef struct device_class
{
	WS_device_init_fn init;
	WS_device_update_fn update;
	WS_device_read_fn read;
	WS_device_send_fn send;
	WS_device_print_fn print;
	WS_device_cmd_stringfy_fn stringfy ;

}WS_dev_vtable_t;

#endif