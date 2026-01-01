#ifndef __DEVICE_EVENT_HEADER__
#define __DEVICE_EVENT_HEADER__

#include "device_message.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct WS_device_packet
{
	int bits;

}WS_dev_pkt_t;

typedef struct WS_device_event
{
	WS_dev_pkt_t *packets;
	size_t size;
}WS_dev_evnt_t;

typedef struct WS_device_interface
{

}WS_dev_infc_t;



#endif