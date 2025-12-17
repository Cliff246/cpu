#ifndef __DEVICE_WCPU__
#define __DEVICE_WCPU__

#include "device_list.h"
#include "device_command.h"
#include "device.h"
#include "core.h"
#include "device_message.h"

#include <stdint.h>
#include <stdbool.h>



typedef struct device_wcpu
{

	core_t *core;

	int port_behaviour;

	bool has_out, has_in;
	bool sent;
	dev_msg_t *current_msg_out;
	dev_msg_t *current_msg_in;
}dev_wcpu_t;


void *device_wcpu_init(device_t *device);

void device_wcpu_update(device_t *dev);
bool device_wcpu_read(WS_dev_t *dev, WS_dev_msg_t *msg);
bool device_wcpu_send(WS_dev_t *dev, WS_dev_msg_t **msg);

void device_wcpu_print(device_t *dev);
WS_dev_cmd_t *device_wcpu_stringfy(toklex_t *tl);
void device_wcpu_commit(WS_dev_t *dev);


#endif
