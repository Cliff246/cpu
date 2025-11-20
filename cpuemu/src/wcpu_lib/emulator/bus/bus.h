#ifndef __BUS_HEADER__
#define __BUS_HEADER__

#include <stdint.h>
#include <stdbool.h>

#include "emucore.h"

typedef enum bus_msg_type
{
	BUS_MSG_INVAL,
	BUS_MSG_READ,
	BUS_MSG_WRITE,
}bus_msg_type_t;

typedef struct bus_msg
{

	int64_t address;
	int64_t data;
	bus_msg_type_t type;
	
}bus_msg_t;

typedef struct bus
{
	int64_t buslimit;

	bus_msg_t *msg;



}bus_t;

bus_t *init_bus(void);

void cycle_bus(emulator_t *emu);


#endif